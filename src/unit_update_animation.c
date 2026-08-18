/* unit_update_animation @0x836D12C8 — per-tick unit animation state machine. First resolves a target
 * "base seat" pose (0-5: none/stand/crouch-or-stand/crouch/prone/melee) from the caller's desired_state byte,
 * with overrides from a scripted magic_seat_index, a "control" flag (0x200), and an active melee-continuous
 * timer (forces melee pose); if that differs from the currently-latched base_seat_index and the current
 * animation allows interruption, re-resolves the unit's seat/weapon animation labels. Then advances four
 * animation_states one tick each via unit_animation_update: soft-ping, the object's base animation, the
 * unit's action animation, and its overlay-action animation.
 *   - the base animation reaching its end (return 1) fires a one-shot side effect keyed by its `state` byte
 *     (melee damage, grenade release, or nothing);
 *   - the base animation looping (return 2) fires a per-state cleanup (destroy-and-respawn or start limp-body
 *     physics for the dead state, a parented-seat visibility handoff, a root-motion-driven seat exit,
 *     stepping the base animation's frame index back one, or forcing `state_desired` to 40 for state 0x27) and, for a
 *     broad range of "busy" states, marks the transition as forced regardless of interruptability;
 *   - the action animation reaching its end/loop kicks off the standard post-action interpolation blend;
 *   - the overlay-action animation reaching its end/loop (2 or 4) while the base state is NOT a horizontal
 *     turn/move (_unit_state_turn_right..._unit_state_move_front, values 3-4) clears its slot.
 * Finally commits `state_desired` via unit_animation_set_state if forced, or if it differs from the current
 * `state` and the current animation is interruptable to it. Returns 1 only when state 0x27 forced
 * `state_desired` to 40 this tick (the caller-visible "just finished dying" signal).
 *
 * DEVIATIONS (disasm-resolved, 0x836D12C8-0x836D1800; no decompiler warning on this one, but several call
 * argument / array-index locals were still lost):
 *  - `base_seat_labels[v12]` (`v12` printed with no visible assignment) is `base_seat_labels[new_base_seat]`
 *    — Hex-Rays lost the connection between the just-computed base-seat value (kept live in r9 across the
 *    whole block) and the array index; disasm confirms the same register feeds both the interruptable check
 *    and the array index.
 *  - `((_cntlzw(data->crouching) & 0x20) == 0) + 2` is the standard "is x nonzero" idiom; simplified to
 *    `data->crouching ? 3 : 2`.
 *  - Two spots in the landing_dead case (0x19, the death/despawn state) read past every modeled struct: an engine-internal
 *    dword at `unit_object + 0x4CC` (4 bytes beyond the full 1228-byte unit_datum) and a dword at
 *    `unit_definition + 0x2F4` (4 bytes past the modelled 372-byte unit_definition_unit). Both are kept as
 *    raw offsets (RAW irreducible: past the modelled struct extents) — their purpose (some per-slot
 *    engine flag / tag-file trailer) isn't load-bearing for anything else in the corpus yet. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/unit_definition.h"
#include "headers/unit_animation_update_data.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/model.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_desired_animation_state.h"
#include "headers/base_seat.h"
#include "headers/unit_definition_flags.h"
#include "headers/unit_animation_overlay_action.h"
#include "headers/animation_update_result.h"
#include "headers/blam_data_globals.h"


typedef struct unit_animation unit_animation;
#include "headers/animation_state.h"
extern const char *unit_get_current_weapon_label(int unit_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern uint8_t unit_animation_state_interruptable(unit_animation *animation, int16_t new_state);
extern int16_t unit_animation_update(int unit_index, int animation_graph_index, animation_state *state);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern void unit_cause_melee_damage(int unit_index, uint8_t silent, int target_object_index, int16_t target_node_index, int16_t target_region_index, int16_t target_material_index, const real_vector3d *target_object_normal);
extern void unit_throw_grenade_release(int unit_index, uint8_t premature);
extern void object_destroy(int object_index);
extern int unit_test_spawning(int unit_index);
extern void biped_start_limp_body_physics(int biped_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void animation_get_root_velocity(const model *model, const animation *animation, int16_t frame_index, real_vector3d *velocity);
extern real_matrix4x3 *object_get_world_matrix(int object_index, real_matrix4x3 *matrix);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern void object_start_interpolation(int object_index, int16_t frame_count);
extern void unit_animation_start_action(int unit_index, int16_t action);

int16_t unit_update_animation(int unit_index, unit_animation_update_data *data)
{
    int16_t just_died = 0;
    int16_t state_desired = data->state_desired;
    uint8_t force_transition = 0;

    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *unit_def = TAG_GET(unit_definition, unit->definition_index);

    /* Resolve the target base-seat pose and, if it changed and the current animation allows it, refresh the
     * unit's seat/weapon animation labels. */
    if (unit->object.parent_object_index == -1 && (unit->object.damage_flags & (1u << _object_dead_bit)) == 0)
    {
        /* desired_state domain is unit_desired_animation_state (DB $5D851471DC20289CC8EC295C9C6E643D);
         * supersedes the earlier BLOCKED adjudication — names are now authoritative. */
        unsigned int desired_state = unit->unit.animation.desired_state;
        int16_t new_base_seat = -1;
        if (desired_state <= _unit_animation_state_flaming)
        {
            switch (desired_state)
            {
            case _unit_animation_state_alert:
            case _unit_animation_state_suspicious: new_base_seat = _base_seat_alert; break;
            case _unit_animation_state_in_combat: new_base_seat = data->crouching ? _base_seat_crouch : _base_seat_stand; break;
            case _unit_animation_state_wary: new_base_seat = _base_seat_stand; break;
            case _unit_animation_state_flee: new_base_seat = _base_seat_flee; break;
            case _unit_animation_state_asleep: new_base_seat = _base_seat_asleep; break;
            default: new_base_seat = _base_seat_flaming; break;
            }
        }

        uint8_t magic_seat_index = unit->unit.magic_seat_index;
        if (magic_seat_index != 0xFF)
            new_base_seat = (char)magic_seat_index;
        if ((unit->unit.control_flags & (1u << _unit_control_force_alert_bit)) != 0)
            new_base_seat = _base_seat_alert;
        if (unit->unit.flaming_death_delay) /* DEVIATION: lbz r10,0x28B @0x836D12C8+ = flaming_death_delay, not melee_continuous_damage_effect_timer (0x28A) */
            new_base_seat = _base_seat_flaming;

        if (unit->unit.animation.base_seat_index != new_base_seat
                && unit_animation_state_interruptable(&unit->unit.animation, state_desired))
        {
            const char *seat_label = base_seat_labels[new_base_seat];
            const char *current_weapon_label = unit_get_current_weapon_label(unit_index);
            unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, current_weapon_label, 1);
        }
    }

    /* All three of the unit's own overlay animation_states (soft-ping, action, overlay-action) advance
     * against the same generic per-object-definition animation graph reference (the tag_reference at
     * definition+68 shared with vehicle/device/object_type_preprocess_node_orientations), NOT the live
     * per-object animation_graph_index the base animation below uses. */
    /* recovered: *(int *)((char *)unit_def + 68) -> unit_def->object.animation_graph.index */
    int overlay_animation_graph_index = unit_def->object.animation_graph.index;

    /* Soft-ping overlay: just advance it. */
    if (unit->unit.animation.soft_ping_animation.index != -1
            && unit_animation_update(unit_index, overlay_animation_graph_index, &unit->unit.animation.soft_ping_animation) == _animation_will_restart_on_next_frame)
    {
        unit->unit.animation.soft_ping_animation.index = -1;
    }

    /* Base animation. */
    if (unit->object.animation.state.index != -1)
    {
        int16_t result = unit_animation_update(unit_index, unit->object.animation.animation_graph_index,
                &unit->object.animation.state);

        if (result == _animation_key_frame)
        {
            /* Base animation finished a one-shot: fire its state-keyed side effect. */
            switch (unit->unit.animation.state)
            {
            case _unit_state_melee_attack:
            case _unit_state_melee_airborne:
            case _unit_state_leap_melee:
                unit_cause_melee_damage(unit_index, 0, -1, -1, -1, -1, 0);
                break;
            case _unit_state_throw_grenade:
                unit_throw_grenade_release(unit_index, 0);
                break;
            default:
                break;
            }
        }
        else if (result == _animation_will_restart_on_next_frame)
        {
            /* Base animation looped: per-state cleanup. */
            switch (unit->unit.animation.state)
            {
            case _unit_state_dying:
            {
                unsigned char should_destroy = 0;
                if ((unit_def->unit.flags & (1u << _unit_definition_destroyed_after_dying_bit)) != 0)
                {
                    if ((unit->object.flags & (1u << _object_at_rest_bit)) != 0)
                    {
                        should_destroy = 1;
                    }
                    else if (unit->object.type == object_type_biped)
                    {
                        /* biped branch: the object is really a biped_datum / biped_definition, whose
                         * biped-class extensions cover these offsets (unit_datum/unit_definition end sooner). */
                        if ((((biped_datum *)unit)->biped.flags & (1u << _biped_airborne_bit)) == 0)
                        {
                            should_destroy = 1;
                        }
                        else
                        {
                            if ((((biped_definition *)unit_def)->biped.flags & (1u << _biped_has_no_dying_airborne_bit)) != 0)
                                should_destroy = 1;
                        }
                    }
                }

                if (should_destroy)
                {
                    object_destroy(unit_index);
                    unit_test_spawning(unit_index);
                }
                else
                {
                    if (unit->object.type == object_type_biped)
                        biped_start_limp_body_physics(unit_index);
                    unit->object.animation.state.frame_index--; /* DEVIATION: 0xD2 is state.frame_index, not the interpolation cursor at 0xD4 (sth r10,0xD2 @0x836D1668) */
                    unit->unit.animation.flags |= (1u << _unit_animation_ignore_translation_bit);
                }
                break;
            }
            case _unit_state_entering_seat: /* parented / vehicle-seat visibility handoff */
            {
                int parent_object_index = unit->object.parent_object_index;
                int16_t parent_seat_index = unit->unit.parent_seat_index;

                unit_datum *parent_object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum);
                unit_definition *parent_def = TAG_GET(unit_definition, parent_object_data->definition_index);

                /* per-seat record is unit_seat (284-byte stride); field 0 is its flags dword.
                 * invisible bit clear means the seat's rider should be visible. */
                int seat_flags = ((unit_seat *)parent_def->unit.seats.address)[parent_seat_index].flags;
                object_set_visibility(unit_index, (seat_flags & (1u << _unit_seat_is_invisible_bit)) == 0);

                if (parent_object_data->unit.driver_object_index == unit_index && parent_object_index != -1)
                    unit_animation_set_state(parent_object_index, _unit_state_closing);
                break;
            }
            case _unit_state_exiting_seat: /* root-motion-driven seat exit */
            {
                animation_graph *base_graph = TAG_GET(animation_graph, unit->object.animation.animation_graph_index);
                /* recovered: *(int *)((char *)unit_def + 52) -> unit_def->object.model.index */
                const model *unit_model = TAG_GET(const model, unit_def->object.model.index);
                const animation *base_anim =
                        &((const animation *)base_graph->animations.address)[unit->object.animation.state.index];

                real_vector3d root_velocity;
                animation_get_root_velocity(unit_model, base_anim, unit->object.animation.state.frame_index, /* DEVIATION: lhz r5,0xD2 @0x836D1690 = state.frame_index */
                        &root_velocity);

                real_matrix4x3 world_matrix;
                object_get_world_matrix(unit_index, &world_matrix);
                matrix4x3_transform_vector(&world_matrix, &root_velocity, &root_velocity);

                unit_exit_seat_end(unit_index, 1, 1, 1);

                unit->object.translational_velocity.n[0] += root_velocity.n[0];
                unit->object.translational_velocity.n[1] += root_velocity.n[1];
                unit->object.translational_velocity.n[2] += root_velocity.n[2];
                break;
            }
            case _unit_state_opening:
            case _unit_state_closing:
                unit->object.animation.state.frame_index--; /* DEVIATION: sth r10,0xD2 @0x836D15A4 = state.frame_index */
                break;
            case _unit_state_leap_start:
                just_died = 1;
                state_desired = _unit_state_leap_airborne;
                break;
            default:
                break;
            }

            /* A broad range of "busy" states always forces the pending state_desired through, regardless of
             * the normal interruptability gate below (disasm-verified ladder, matches the decompiler's own
             * rendering exactly). */
            unsigned int state = unit->unit.animation.state;
            uint8_t interruptable = 1;
            if (state > _unit_state_resurrect_back)
            {
                if (state == _unit_state_leap_start || state == _unit_state_leap_melee)
                    interruptable = 0;
            }
            else if (state >= _unit_state_throw_grenade)
            {
                interruptable = 0;
            }
            else if (state <= _unit_state_hard_ping)
            {
                if (state == _unit_state_hard_ping || (state != _unit_state_idle && state <= _unit_state_turn_right))
                    interruptable = 0;
            }
            else if (state >= _unit_state_entering_seat && state <= _unit_state_melee_airborne)
            {
                interruptable = 0;
            }

            if (!interruptable)
                force_transition = 1;
        }
    }

    /* Action animation: on end/loop, kick off the standard post-action interpolation blend. */
    if (unit->unit.animation.action_animation.index != -1
            && unit_animation_update(unit_index, overlay_animation_graph_index, &unit->unit.animation.action_animation) == _animation_will_restart_on_next_frame)
    {
        object_start_interpolation(unit_index, 6);
        unit_animation_start_action(unit_index, 0);
    }

    /* Overlay-action animation: on end/loop, clear it unless the unit's base state is a horizontal
     * turn/move (states _unit_state_turn_right(3)..._unit_state_move_front(4)), where the overlay is kept. */
    if (unit->unit.animation.overlay_action_animation.index != -1)
    {
        int16_t result = unit_animation_update(unit_index, overlay_animation_graph_index,
                &unit->unit.animation.overlay_action_animation);
        if (result == _animation_will_restart_on_next_frame || result == _animation_looped)
        {
            unsigned int state = unit->unit.animation.state;
            uint8_t keep = state >= _unit_state_turn_right && state <= _unit_state_move_front;
            if (!keep)
            {
                unit->unit.animation.overlay_action_animation.index = -1;
                unit->unit.animation.overlay_action = _unit_overlay_action_none;
            }
        }
    }

    if (force_transition
            || (state_desired != unit->unit.animation.state
                    && unit_animation_state_interruptable(&unit->unit.animation, state_desired)))
    {
        unit_animation_set_state(unit_index, state_desired);
    }

    return just_died;
}
