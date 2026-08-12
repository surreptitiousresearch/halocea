/* weapon_update @0x836DD328 — per-frame update of a single weapon object. Resolves the weapon object
 * datum and its weapon-definition tag, then advances every piece of the weapon's runtime state:
 *   - ticks the weapon's animation (ejecting a shell / clearing state on completion),
 *   - detonates weapons flagged "detonate when dropped" that have no parent,
 *   - decays weapon.integrated_light_power unless held by a unit with the "no battery decay" flag,
 *   - runs the overheat state machine (weapon.heat vs tag overheated/detonation thresholds), spawning
 *     and stopping the looping overheat effect and possibly detonating,
 *   - computes each trigger's "wants to fire" input (primary/secondary), gated by weapon secondary
 *     modes,
 *   - services every magazine: reload progress (rounds_loaded / rounds_fractional_recharged in units
 *     of 30), reload/chamber timers, and reload-state transitions,
 *   - runs each trigger's firing state machine: begin-firing, charging (build/hold/release/overcharge),
 *     firing, and the auto-reload trigger, updating weapon.triggers[t] and interpolating its visual
 *     amounts (rate_of_fire / ejection_port_position / illumination / __noop) against the trigger
 *     definition's runtime_* rates.
 *
 * Typed access (DB-verified 2026-07-14): datum via weapon_datum (object/item/weapon layers), tag via
 * weapon_definition; magazine runtime = weapon_magazine, trigger runtime = weapon_trigger, and the
 * per-magazine / per-trigger tag blocks are weapon_magazine_definition (112 bytes) and
 * weapon_trigger_definition (276 bytes).
 *
 * CAVEAT: (1) The decompiler packed several unrelated scratch values into one shared 64-bit slot; these
 * have been untangled into separate descriptively-named locals verified against the disassembly.
 * (2) The bare `weapon_get_owner_object_index(weapon_index);` call discards its result in the binary
 * (the effect owner index is taken from the weapon/parent computed just above) — reproduced verbatim.
 * (3) The magazine reload split used a magic-number division (2290649225*x>>32...) which is exactly
 * x/30; rewritten as /30 and %30. (4) The object_permute_region region-name argument is
 * blurred_permutation_names[trigger_index] (a global char* array); the decompiler lost it because the
 * compiler hoisted the load across the weapon_get_effect_object_index call, which preserves r4.
 * (5) The decompiler re-loads the weapon datum pointer from object_header_data at many sites; all of
 * those reloads resolve to the same element, so the single `weapon` pointer is reused. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/animation_state.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_definition_flags.h"
#include "headers/weapon_trigger_flags.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/networked_datum_role.h"
#include "headers/object_flags.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_trigger_state.h"
#include "headers/weapon_datum_state.h"
#include "headers/weapon_datum_flags.h"
#include "headers/weapon_control_flags.h"
#include "headers/weapon_definition_flags.h"
#include "headers/weapon_secondary_trigger_mode.h"
#include "headers/weapon_overcharged_action.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/item_flags.h"
#include "headers/weapon_type.h"
#include "headers/object_type.h"
#include "headers/animation_update_result.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

#include "headers/animation_state.h"
#include "headers/animation_update_kind.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t animation_update_internal(animation_update_kind render_or_affects_game_state, int animation_graph_index, animation_state *state, int *triggered_sound_index);
extern void weapon_trigger_start_ejection_port(int weapon_index, int16_t trigger_index, uint8_t chamber);
extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern void item_detonate(int item_index);
extern void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type);
extern int weapon_get_owner_object_index(int weapon_index);
extern int effect_new_looping(int definition_index, int object_index, int16_t scale_a_function_index, int16_t scale_b_function_index, int16_t change_color_index);
extern void effect_stop(int effect_index, uint8_t and_delete);
extern void weapon_magazine_start_reload(int weapon_index, int16_t magazine_index, uint8_t first_round);
extern void weapon_magazine_finish_reload(int weapon_index, int16_t magazine_index);
extern void weapon_magazine_start_chamber(int weapon_index, int16_t magazine_index);
extern void weapon_trigger_begin_firing(int weapon_index, int16_t trigger_index, uint8_t force_firing);
extern void weapon_trigger_fire(int weapon_index, int16_t trigger_index);
extern void weapon_trigger_release_charge(int weapon_index, int16_t trigger_index);
extern void weapon_detonate(int weapon_index);
extern void hcex_obj_set_state(int object_index, const char *state_name);
extern uint8_t weapon_trigger_can_fire_again(int weapon_index, int16_t trigger_index);
extern int weapon_get_effect_object_index(int weapon_index);
extern void object_permute_region(int object_index, const char *permutation_name, int16_t desired_region_index, uint8_t active_flag);

extern void weapon_trigger_finish_tracking(int weapon_index, int16_t trigger_index);
uint8_t weapon_update(int weapon_index) /* was: int — DB prototype */
{
    weapon_datum *weapon = (weapon_datum *)DATUM_GET(object_header_data, object_header_datum,
                                                              weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    if ( g_running_simulated_update == 1 )
        return 1;

    /* invalidate the tracked object index if it no longer resolves to a live object */
    int tracked_object_index = weapon->weapon.tracked_object_index;
    if ( tracked_object_index != -1 && !object_try_and_get_and_verify_type(tracked_object_index, object_mask_all) )
        weapon->weapon.tracked_object_index = -1;

    /* advance the weapon's own animation */
    int animation_graph_index = definition->object.animation_graph.index;
    if ( animation_graph_index != -1 && (uint16_t)weapon->object.animation.state.index != 0xFFFF )
    {
        int16_t animation_result = animation_update_internal(animation_update_kind_affects_game_state,
                                                             animation_graph_index,
                                                             &weapon->object.animation.state, 0);
        if ( animation_result == _animation_key_frame )
        {
            /* animation finished: eject a shell for chambering states */
            int animation_state_value = (uint8_t)weapon->weapon.state;
            int16_t ejection_barrel_index;
            if ( animation_state_value == weapon_state_primary_chamber )
                ejection_barrel_index = 0;
            else if ( animation_state_value == weapon_state_secondary_chamber )
                ejection_barrel_index = 1;
            else
                goto animation_done;
            weapon_trigger_start_ejection_port(weapon_index, ejection_barrel_index, 1);
        }
        else if ( animation_result == _animation_will_restart_on_next_frame )
        {
            /* animation interrupted: drop back to idle unless in a locked ready/put-away state */
            unsigned int animation_state_value = (uint8_t)weapon->weapon.state;
            if ( animation_state_value < weapon_state_primary_charged
                 || animation_state_value > weapon_state_secondary_charged
                    && animation_state_value != weapon_state_put_away )
                weapon_set_state(weapon_index, weapon_state_idle, 1);
        }
    }
animation_done:

    /* "detonate when dropped": tag flag bit 10, no parent object */
    if ( (definition->weapon.flags & (1u << _weapon_detonates_when_dropped_bit)) != 0 && weapon->object.parent_object_index == -1 )
        item_detonate(weapon_index);

    /* battery / integrated light power decay — suppressed while held by a unit whose definition sets
     * bit 23 (_unit_integrated_light_controls_weapon_directly) */
    if ( weapon->weapon.integrated_light_power > 0.0 )
    {
        int parent_object_index = weapon->object.parent_object_index;
        int allow_battery_decay = 1;
        if ( parent_object_index != -1 )
        {
            int *parent_object_data = object_try_and_get_and_verify_type(parent_object_index, object_mask_unit);
            if ( parent_object_data )
            {
                if ( *parent_object_data != -1 )
                {
                    unit_definition *parent_definition = TAG_GET(unit_definition, *parent_object_data);
                    allow_battery_decay = (parent_definition->unit.flags & (1u << _unit_integrated_light_controls_weapon_directly)) == 0;
                }
            }
        }
        if ( allow_battery_decay )
        {
            float new_battery = (weapon->weapon.integrated_light_power - (float)0.041666668);
            weapon->weapon.integrated_light_power = weapon->weapon.integrated_light_power - (float)0.041666668;
            if ( new_battery < 0.0 )
                weapon->weapon.integrated_light_power = 0.0;
        }
    }

    /* overheat state machine */
    float heat = weapon->weapon.heat;
    if ( heat > 0.0 )
    {
        if ( heat >= definition->weapon.heat_overheated_threshold )
        {
            unsigned int heat_flags = weapon->weapon.flags;
            if ( (heat_flags & (1u << _weapon_overheated_bit)) == 0 )
            {
                unsigned int overheated_flags = heat_flags | (1u << _weapon_overheated_bit);
                weapon->weapon.flags = overheated_flags;

                int16_t overheat_message;
                if ( definition->weapon.weapon_type == _weapon_type_plasma_pistol
                     && (overheated_flags & (1u << _weapon_overheat_recoil_bit)) != 0 )
                {
                    overheat_message = _first_person_weapon_message_overheating_super_recoil;
                    weapon->weapon.flags = overheated_flags & ~(1u << _weapon_overheat_recoil_bit);
                }
                else
                {
                    overheat_message = _first_person_weapon_message_overheating;
                }
                first_person_weapon_message_from_weapon(weapon_index, overheat_message);

                /* spawn the looping overheat effect on the weapon (or its holder) */
                int overheat_effect_index = -1;
                int overheat_effect_definition = definition->weapon.overheated_effect.index;
                if ( overheat_effect_definition != -1 )
                {
                    int effect_object_index = weapon_index;
                    if ( (weapon->object.flags & (1u << _object_invisible_bit)) != 0 )
                    {
                        int parent = weapon->object.parent_object_index;
                        if ( parent != -1 )
                            effect_object_index = parent;
                    }
                    weapon_get_owner_object_index(weapon_index);   /* result discarded in binary */
                    if ( effect_object_index != -1 )
                        overheat_effect_index = effect_new_looping(overheat_effect_definition,
                                                                   effect_object_index, -1, -1, -1);
                }
                weapon->weapon.overheated_effect_index = overheat_effect_index;
            }
        }

        /* cool down once per frame */
        if ( weapon->weapon.overcharged == 0.0 )
        {
            float heat_decrement = (definition->weapon.heat_loss_per_second * SECONDS_PER_TICK);
            if ( definition->weapon.age_heat_recovery_penalty > 0.0 )
                heat_decrement = (-((weapon->weapon.age
                                                                 * definition->weapon.age_heat_recovery_penalty)
                                                         - (float)1.0)
                                         * (definition->weapon.heat_loss_per_second * (float)0.033333335));
            float new_heat = (weapon->weapon.heat - heat_decrement);
            weapon->weapon.heat = weapon->weapon.heat - heat_decrement;
            if ( new_heat < 0.0 )
                weapon->weapon.heat = 0.0;

            unsigned int cool_flags = weapon->weapon.flags;
            if ( (cool_flags & (1u << _weapon_overheated_bit)) != 0
                && (cool_flags & (1u << _weapon_overheated_exit_bit)) == 0
                && ((weapon->weapon.heat - definition->weapon.heat_recovery_threshold)
                           / heat_decrement) <= 1.0 )
            {
                weapon->weapon.flags = cool_flags | (1u << _weapon_overheated_exit_bit);
            }
        }

        /* leave the overheated state once heat drops below the venting threshold */
        unsigned int vent_flags = weapon->weapon.flags;
        if ( (vent_flags & (1u << _weapon_overheated_bit)) != 0
             && weapon->weapon.heat < (double)definition->weapon.heat_recovery_threshold )
        {
            int overheat_effect_index = weapon->weapon.overheated_effect_index;
            weapon->weapon.flags = vent_flags
                                   & ~((1u << _weapon_overheated_bit) | (1u << _weapon_overheated_exit_bit));
            if ( overheat_effect_index != -1 )
                effect_stop(overheat_effect_index, 1);
        }
    }

    /* trigger-input latch: tick down the "just fired" countdown and clear the per-frame cool lock */
    int16_t fire_countdown = weapon->weapon.state_timer;
    weapon->weapon.overcharged = 0.0;
    if ( fire_countdown > 0 )
        weapon->weapon.state_timer = fire_countdown - 1;

    /* determine which triggers the player/AI wants to fire this frame (up to 2 triggers) */
    uint16_t trigger_input_flags = weapon->weapon.control_flags;
    char trigger_input_low = (char)trigger_input_flags;
    char secondary_wants_fire;
    char trigger_should_fire[2];
    if ( (trigger_input_flags & (1u << _weapon_control_user_busy_bit)) != 0
         || weapon->weapon.state_timer > 0 )
    {
        secondary_wants_fire = 0;
        trigger_should_fire[0] = 0;
    }
    else
    {
        /* DB bit 12 = _weapon_secondary_trigger_overrides_grenades (gates secondary-trigger firing here) */
        unsigned int dual_wield_flag = definition->weapon.flags & (1u << _weapon_secondary_trigger_overrides_grenades_bit);
        trigger_should_fire[0] = (trigger_input_flags & (1u << _weapon_control_primary_trigger_bit)) != 0;
        secondary_wants_fire = (dual_wield_flag != 0
                                && (trigger_input_flags & (1u << _weapon_control_secondary_trigger_bit)) != 0)
                               ? 1 : 0;
    }

    int secondary_trigger_mode = (uint16_t)definition->weapon.secondary_trigger_mode;
    trigger_should_fire[1] = secondary_wants_fire;
    if ( secondary_trigger_mode == _weapon_secondary_trigger_mode_slaved_to_primary )
    {
        if ( secondary_wants_fire && definition->weapon.triggers.count > 0
             && weapon->weapon.triggers[0].rate_of_fire != 1.0 )
            trigger_should_fire[1] = 0;
    }
    else if ( secondary_trigger_mode == _weapon_secondary_trigger_mode_inhibits_primary && secondary_wants_fire )
    {
        trigger_should_fire[0] = 0;
    }

    /* honor a queued reload request (input bit 0x8) for authoritative weapons */
    if ( weapon->object.datum_role != _networked_datum_puppet
         && (trigger_input_low & (1 << _weapon_control_reload_bit)) != 0
         && definition->weapon.magazines.count > 0 )
        weapon->weapon.flags |= (1u << _weapon_needs_to_reload_bit);
    if ( (weapon->weapon.flags & (1u << _weapon_needs_to_reload_bit)) != 0 )
        weapon_magazine_start_reload(weapon_index, 0, 1);

    /* service every magazine: reload progress and reload/chamber timers */
    int16_t magazine_index = 0;
    if ( definition->weapon.magazines.count > 0 )
    {
        int magazine = 0;
        do
        {
            weapon_magazine_definition *magazine_definition =
                &((weapon_magazine_definition *)definition->weapon.magazines.address)[magazine];
            weapon_magazine *magazine_state_block = &weapon->weapon.magazines[magazine];
            int16_t reload_increment = magazine_definition->rounds_recharged_per_second;
            if ( reload_increment > 0 )
            {
                int loaded_rounds = (uint16_t)magazine_state_block->rounds_loaded;
                if ( (int16_t)loaded_rounds < magazine_definition->rounds_loaded_maximum )
                {
                    int loaded_fraction = (uint16_t)magazine_state_block->rounds_fractional_recharged;
                    /* rounds are stored as whole rounds plus a /30 fractional part;
                     * magic-number division by 30 in the binary rewritten as /30 and %30 */
                    int new_loaded = reload_increment / 30 + loaded_rounds;
                    magazine_state_block->rounds_loaded = new_loaded;
                    int new_fraction = reload_increment % 30 + loaded_fraction;
                    magazine_state_block->rounds_fractional_recharged = new_fraction;
                    if ( (int16_t)new_fraction >= 30 )
                    {
                        magazine_state_block->rounds_fractional_recharged = new_fraction - 30;
                        magazine_state_block->rounds_loaded = new_loaded + 1;
                    }
                    int16_t magazine_capacity = magazine_definition->rounds_loaded_maximum;
                    if ( magazine_state_block->rounds_loaded > magazine_capacity )
                        magazine_state_block->rounds_loaded = magazine_capacity;
                }
            }

            if ( magazine_state_block->state_timer )
                --magazine_state_block->state_timer;

            int magazine_state = (uint16_t)magazine_state_block->state;
            if ( magazine_state == _weapon_magazine_reloading )
            {
                if ( magazine_state_block->state_timer - 1 <= 0 )
                    weapon_magazine_finish_reload(weapon_index, magazine_index);
            }
            else if ( magazine_state == _weapon_magazine_reloaded )
            {
                weapon_magazine_start_chamber(weapon_index, magazine_index);
            }
            else if ( magazine_state == _weapon_magazine_chambering && !magazine_state_block->state_timer )
            {
                magazine_state_block->state = _weapon_magazine_idle;
                magazine_state_block->state_timer = 0;
            }

            magazine_index = (int16_t)(magazine + 1);
            magazine = magazine_index;
        }
        while ( magazine_index < definition->weapon.magazines.count );
    }

    /* service every trigger: input latch, firing state machine, and visual interpolation */
    int16_t trigger_index = 0;
    if ( definition->weapon.triggers.count > 0 )
    {
        int trigger = 0;
        do
        {
            weapon_trigger_definition *trigger_definition =
                &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger];
            weapon_trigger *trigger_state = &weapon->weapon.triggers[trigger];
            unsigned int trigger_def_flags = trigger_definition->flags;

            /* analog rate of fire: firing is driven by the analog trigger amount being non-trivial */
            if ( (trigger_def_flags & (1u << _weapon_trigger_analog_rate_of_fire_bit)) != 0 )
            {
                if ( (weapon->item.flags & (1u << _item_belongs_to_player_bit)) != 0 )
                    trigger_should_fire[trigger] = weapon->weapon.primary_trigger > 0.050000001;
            }
            /* sticks when dropped: auto-fires while unheld (no parent) */
            if ( (trigger_def_flags & (1u << _weapon_trigger_sticks_when_dropped_bit)) != 0
                 && weapon->object.parent_object_index == -1 )
                trigger_should_fire[trigger] = 1;

            if ( trigger_state->state_timer )
                --trigger_state->state_timer;

            /* trigger def flag 0x10: latch the "held" bit (0x4) and re-derive want-to-fire from it */
            if ( (trigger_definition->flags & (1u << _weapon_trigger_toggles_bit)) != 0 )
            {
                unsigned int trigger_flags = trigger_state->flags;
                if ( (trigger_flags & (1u << _weapon_trigger_was_down_bit)) == 0 && trigger_should_fire[trigger] )
                    trigger_state->flags = trigger_flags ^ (1u << _weapon_trigger_toggled_bit);
                unsigned int latched_flags = trigger_state->flags;
                if ( trigger_should_fire[trigger] )
                    latched_flags |= (1u << _weapon_trigger_was_down_bit);
                else
                    latched_flags &= ~(1u << _weapon_trigger_was_down_bit);
                trigger_state->flags = latched_flags;
                trigger_should_fire[trigger] = (latched_flags >> _weapon_trigger_toggled_bit) & 1;
            }

            BOOL trigger_firing = trigger_should_fire[trigger];
            if ( !trigger_should_fire[trigger] )
                trigger_state->flags |= (1u << _weapon_trigger_released_since_last_shot_bit);

            BOOL firing_done;

            if ( trigger_state->ejection_port_position > 0.0 )
            {
                float new_ejection_port = (trigger_state->ejection_port_position
                                                  - trigger_definition->runtime_ejection_port_recovery_time);
                trigger_state->ejection_port_position = trigger_state->ejection_port_position
                                                        - trigger_definition->runtime_ejection_port_recovery_time;
                if ( new_ejection_port <= 0.0 )
                    trigger_state->ejection_port_position = 0.0;
            }
            if ( trigger_state->illumination > 0.0 )
            {
                float new_illumination = (trigger_state->illumination
                                                 - trigger_definition->runtime_illumination_recovery_time);
                trigger_state->illumination = trigger_state->illumination
                                              - trigger_definition->runtime_illumination_recovery_time;
                if ( new_illumination <= 0.0 )
                    trigger_state->illumination = 0.0;
            }

            unsigned int firing_state = (uint8_t)trigger_state->state;
            if ( firing_state <= _weapon_trigger_ready )
            {
                if ( firing_state == _weapon_trigger_firing )   /* firing */
                {
                    if ( !trigger_firing )
                    {
                        weapon_trigger_begin_firing(weapon_index, trigger_index, 1);
                        goto trigger_interpolate;
                    }
                    if ( !trigger_state->state_timer
                         && weapon->weapon.alternate_shots_loaded < definition->weapon.maximum_alternate_shots_loaded )
                    {
                        if ( trigger + 1 < definition->weapon.triggers.count )
                            weapon_trigger_fire(weapon_index, trigger + 1);
                        int16_t fire_duration = (int)(trigger_definition->overloading_time * (float)30.0);
                        trigger_state->state = _weapon_trigger_firing;
                        trigger_state->state_timer = fire_duration;
                    }
                }
                else if ( firing_state == _weapon_trigger_charging )   /* charging (building up) */
                {
                    if ( trigger_state->state_timer )
                    {
                        if ( trigger_firing )
                            goto trigger_charge_up;
                        /* charge interrupted */
                        if ( trigger || definition->weapon.triggers.count <= 1
                             || (trigger_state->flags & (1u << _weapon_trigger_fired_before_charging_bit)) != 0 )
                        {
                            trigger_state->state = _weapon_trigger_idle;
                            trigger_state->state_timer = 0;
                        }
                        else
                        {
                            weapon_trigger_begin_firing(weapon_index, trigger_index, 1);
                        }
                        int charge_effect_index = trigger_state->charging_effect_index;
                        if ( charge_effect_index != -1 )
                        {
                            effect_stop(charge_effect_index, 1);
                            trigger_state->charging_effect_index = -1;
                        }
                        hcex_obj_set_state(weapon_index, "charging_stop");
                    }
                    else   /* charge complete -> transition to hold */
                    {
                        float charge_hold_time = trigger_definition->charged_time;
                        trigger_state->state = _weapon_trigger_charged;
                        trigger_state->state_timer = (int)(charge_hold_time * (float)30.0);
                        /* trigger 0 -> weapon_state_primary_charged (7), trigger 1 -> secondary_charged (8) */
                        weapon_set_state(weapon_index, weapon_state_primary_charged + trigger, 1);
                        first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_charged);
                    }
                }
                else if ( firing_state == _weapon_trigger_charged && trigger_state->state )   /* charged: holding */
                {
                    if ( !trigger_firing )
                        goto trigger_release_charge;
                    int16_t charge_hold_timer = trigger_state->state_timer;
                    float charge_fraction = ((float)1.0
                                                    - (((float)charge_hold_timer * SECONDS_PER_TICK)
                                                              / trigger_definition->charged_time));
                    weapon->weapon.overcharged = charge_fraction;
                    if ( trigger_state->state_timer )
                    {
                        if ( weapon->weapon.magazines[trigger_definition->magazine_index].rounds_loaded
                                 < trigger_definition->rounds_per_shot
                             && (trigger_definition->flags & (1u << _weapon_trigger_can_fire_with_partial_ammunition_bit)) == 0 )
                        {
trigger_release_charge:
                            weapon_trigger_release_charge(weapon_index, trigger_index);
                        }
                    }
                    else   /* charge held to the limit: overcharge action */
                    {
                        int overcharge_action = (uint16_t)trigger_definition->overcharged_action;
                        if ( overcharge_action == _weapon_overcharged_action_explode )
                            weapon_detonate(weapon_index);
                        else if ( overcharge_action == _weapon_overcharged_action_discharge )
                            goto trigger_release_charge;
                    }
                }
                else if ( firing_state == _weapon_trigger_recovering && trigger_state->state )   /* GUESS: post-shot recovery */
                {
                    if ( trigger_state->state_timer )
                        goto trigger_interpolate;
                    if ( (trigger_definition->flags & (1u << _weapon_trigger_latched_bit)) != 0
                         && (weapon->item.flags & (1u << _item_belongs_to_player_bit)) != 0
                         && (trigger_state->flags & (1u << _weapon_trigger_released_since_last_shot_bit)) == 0 )
                    {
                        trigger_state->state = _weapon_trigger_firing_held;
                        trigger_state->state_timer = -1;
                        goto trigger_interpolate;
                    }
                    goto reset_firing_state;
                }
                else if ( firing_state == _weapon_trigger_tracking && trigger_state->state )   /* GUESS: homing lock held */
                {
                    /* DEVIATION: inlined+ICF-merged copy of weapon_trigger_finish_tracking@0x836DC288 (zero-xref donor) collapsed to a call; goto retargeted from clear_firing_block to trigger_interpolate because the call already performs the tail's state=0/state_timer=0 resets (offsets 0x261/0x262 verified identical at disasm grade) that clear_firing_block also does for two unrelated paths. */
                    if ( !trigger_firing || weapon->weapon.tracked_object_index == -1 )
                    {
                        weapon_trigger_finish_tracking(weapon_index, trigger_index);
                        goto trigger_interpolate;
                    }
                }
                else if ( firing_state == _weapon_trigger_releasing && trigger_state->state )   /* releasing: spew charge */
                {
                    if ( trigger_state->state_timer )
                    {
                        weapon_trigger_begin_firing(weapon_index, trigger_index, 1);
                        goto trigger_interpolate;
                    }
                    trigger_state->idle_ticks = 0;
                    goto clear_firing_block;
                }
                else if ( firing_state == _weapon_trigger_firing_held && trigger_state->state )   /* GUESS: continuous held fire */
                {
                    firing_done = !trigger_firing;
                    goto check_firing_done;
                }
                else if ( trigger_state->state )   /* other active states */
                {
                    firing_done = trigger_state->state_timer == 0;
check_firing_done:
                    if ( !firing_done )
                        goto trigger_interpolate;
reset_firing_state:
clear_firing_block:
                    trigger_state->state_timer = 0;
                    trigger_state->state = _weapon_trigger_idle;
                    goto trigger_interpolate;
                }
                else   /* firing_state == _weapon_trigger_idle: idle — check for auto-reload and re-fire */
                {
                    unsigned char can_fire = 1;
                    if ( (weapon->weapon.control_flags & (1u << _weapon_control_user_busy_bit)) == 0
                         && weapon->object.parent_object_index != -1 )
                    {
                        int magazine_for_trigger = trigger_definition->magazine_index;
                        if ( magazine_for_trigger != -1 )
                        {
                            int rounds_needed = trigger_definition->rounds_per_shot;
                            weapon_magazine *trigger_magazine = &weapon->weapon.magazines[magazine_for_trigger];
                            int16_t loaded = trigger_magazine->rounds_loaded;
                            if ( loaded < rounds_needed && (trigger_definition->flags & (1u << _weapon_trigger_can_fire_with_partial_ammunition_bit)) == 0
                                 || loaded < trigger_definition->minimum_rounds_loaded_per_shot
                                 || !(uint16_t)trigger_magazine->rounds_loaded )
                            {
                                char do_reload = 0;
                                if ( !weapon->object.datum_role && rounds_needed > 0 )
                                {
                                    char autoreload_counter = trigger_state->delay_ticks_before_empty_clip_auto_reload + 1;
                                    trigger_state->delay_ticks_before_empty_clip_auto_reload = autoreload_counter;
                                    /* can_fire = (reserve rounds > 0) */
                                    can_fire = (~((unsigned int)trigger_magazine->rounds_total >> 31)
                                                + ((uint16_t)trigger_magazine->rounds_total != 0)) & 1;
                                    if ( autoreload_counter > 10 )
                                    {
                                        can_fire = 1;
                                        trigger_state->delay_ticks_before_empty_clip_auto_reload = 0;
                                        do_reload = 1;
                                    }
                                }
                                if ( weapon->object.datum_role == _networked_datum_autonomous )
                                    do_reload = 1;
                                if ( do_reload == 1 )
                                    weapon_magazine_start_reload(weapon_index,
                                                                 (uint16_t)trigger_definition->magazine_index, 1);
                            }
                        }
                    }
                    if ( trigger_firing && weapon_trigger_can_fire_again(weapon_index, trigger_index)
                         && can_fire == 1 )
                    {
                        weapon_trigger_begin_firing(weapon_index, trigger_index, 0);
                    }
                    else if ( trigger_state->idle_ticks != 127 )
                    {
                        ++trigger_state->idle_ticks;
                    }
                }
            }
            goto trigger_interpolate;

trigger_interpolate:
            if ( !trigger_firing )
            {
                float new_rate_of_fire = (trigger_state->rate_of_fire
                                                 - trigger_definition->runtime_rate_of_fire_deceleration_time);
                trigger_state->rate_of_fire = trigger_state->rate_of_fire
                                              - trigger_definition->runtime_rate_of_fire_deceleration_time;
                if ( new_rate_of_fire < 0.0 )
                    trigger_state->rate_of_fire = 0.0;
                if ( (trigger_state->flags & (1u << _weapon_trigger_blurred_bit)) == 0
                     || trigger_state->rate_of_fire >= (double)trigger_definition->blurred_rate_of_fire )
                {
                    goto trigger_effect_intensity;
                }
                /* rate of fire dropped below the "blurred" threshold: hide the blur region permutation */
                int effect_object_index = weapon_get_effect_object_index(weapon_index);
                object_permute_region(effect_object_index, blurred_permutation_names[trigger], -1, 0);
                trigger_state->flags = trigger_state->flags & ~(1u << _weapon_trigger_blurred_bit);
                goto trigger_effect_intensity;
            }

trigger_charge_up:
            {
                float new_rate_of_fire = (trigger_definition->runtime_rate_of_fire_acceleration_time
                                                 + trigger_state->rate_of_fire);
                trigger_state->rate_of_fire = trigger_definition->runtime_rate_of_fire_acceleration_time
                                              + trigger_state->rate_of_fire;
                if ( new_rate_of_fire > 1.0 )
                    trigger_state->rate_of_fire = 1.0;
                float blur_threshold = trigger_definition->blurred_rate_of_fire;
                if ( blur_threshold == 0.0 || (trigger_state->flags & (1u << _weapon_trigger_blurred_bit)) != 0
                     || trigger_state->rate_of_fire <= blur_threshold )
                    goto trigger_effect_intensity;
                /* rate of fire rose above the "blurred" threshold: show the blur region permutation */
                int effect_object_index = weapon_get_effect_object_index(weapon_index);
                object_permute_region(effect_object_index, blurred_permutation_names[trigger], -1, 1);
                trigger_state->flags = trigger_state->flags | (1u << _weapon_trigger_blurred_bit);
            }

trigger_effect_intensity:
            {
                char active_firing_state = trigger_state->state;
                if ( active_firing_state == _weapon_trigger_releasing || active_firing_state == _weapon_trigger_recovering || trigger_firing )
                {
                    float new_error = (trigger_definition->runtime_error_acceleration_time
                                              + trigger_state->__noop);
                    trigger_state->__noop = trigger_definition->runtime_error_acceleration_time
                                            + trigger_state->__noop;
                    if ( new_error > 1.0 )
                        trigger_state->__noop = 1.0;
                }
                else
                {
                    float new_error = (trigger_state->__noop
                                              - trigger_definition->runtime_error_deceleration_time);
                    trigger_state->__noop = trigger_state->__noop
                                            - trigger_definition->runtime_error_deceleration_time;
                    if ( new_error < 0.0 )
                        trigger_state->__noop = 0.0;
                }
            }

            trigger_index = (int16_t)(trigger + 1);
            trigger = trigger_index;
        }
        while ( trigger_index < definition->weapon.triggers.count );
    }
    return 1;
}
