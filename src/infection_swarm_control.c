/* infection_swarm_control @0x83827710 — per-tick AI controller for a flood "infection" swarm: an actor
 * that drives a group of small units (its swarm_datum) as a loose flock. Once per intermittent-action
 * cycle it (re)seeds a random action timer and picks one swarm member to perform a special action. Then,
 * for every member, it: reads the member's biped state (up/ground reference), optionally scans the actor's
 * props to pick the best combat target, decides a movement mode from the actor's action state, handles
 * attach/detach to a host unit, builds a movement/facing direction (wander toward the swarm centre or
 * pursue/flee a target), applies neighbour-avoidance steering, orthonormalises the direction against the
 * ground/up reference, and finally emits a unit_control packet (throttle + facing/aiming/looking vectors,
 * jump/lunge trigger, animation grip state) via unit_control().
 *
 * Types resolved from the DB: v1→actor_datum, swarm_data slot→swarm_datum, swarm_component_data slot→
 * swarm_component_datum, the object-pool lookups→biped_datum / unit_datum, prop scan→prop_datum, the
 * actor variant tag→actor_variant_definition, control packet→unit_control_data.
 *
 * DEVIATIONS (marked inline):
 *  - The prop-scan reads the iterator's CURRENT prop index. The decompiler labelled it HIDWORD(v166)
 *    (offset 4, next_index); the disassembly (lwz r10,var_1D0 = offset 0) shows it is prop_iter.index.
 *  - The unit_control packet is zero-filled by an unrolled 8x qword store the decompiler rendered as a
 *    "v154 & 0x200000000" fill; that source value is 0 (r14) — modelled as memset(&control,0,sizeof).
 *    control_flags is then set to the jump bit (the infection lunge; verified via
 *    unit_control -> unit.control_flags -> biped_update_jumping) when should_jump, else 0.
 *  - The detach-decision block for an inactive host was physically emitted after the loop and re-entered
 *    via goto; it is inlined here at the point the decompiler breaks to it (no irreducible control flow).
 *  - swarm_component_datum union work-area (wander vs obey) fields are DB-typed; the obey-branch steering
 *    reads (component+0x24/0x28/0x2C/0x30) are obey.___u5.directmovement.facing / .vector.n[0..2].
 *  - Redundant single-precision (float) casts from the decompiler are dropped (all operands are float).
 *  - biped.flags bit 0 is AIRBORNE (see biped_datum_flags.h) — an earlier pass had the polarity
 *    inverted ("biped_on_ground"); the corrected reading is that steering is skipped while airborne.
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/biped_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/unit_definition.h"
#include "headers/unit_control_data.h"
#include "headers/real_vector3d.h"
#include "headers/actor_action.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_definition_flags.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/unit_desired_animation_state.h"
#include "headers/biped_datum_flags.h"
#include "headers/swarm_component_flags.h"
#include "headers/obey_simple_control_flags.h"
#include "headers/swarm_movement_mode.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern int game_time_get(void);
extern void unit_detach_from_parent(int object_index);
extern uint8_t unit_is_busy(int object_index);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern float __fsqrts(float x);
extern float __fabs(float x);
extern double cos(double x);
extern double sin(double x);
extern void *memset(void *destination, int value, unsigned int size);

/* Address of the live object-pool datum pointer for an object index (salted 12-byte entry, pointer at +2 dwords). */

void infection_swarm_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
    actor_variant_definition *variant =
            TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);

    const float action_duration_scale = 30.0f;
    int16_t intermittent_action_member = -1;

    /* Once per cycle: refresh the intermittent-action timer and pick a member to perform it. */
    if ( swarm->swarm_intermittent_action_timer <= 0 )
    {
        int16_t action = actor->state.action;
        if ( action == actor_action_search || action == actor_action_charge )
        {
            float duration = real_seed_random_range(get_global_random_seed_address(), 6.0f, 8.0f);
            int16_t unit_count = swarm->unit_count;
            float new_timer = duration / (float)unit_count * action_duration_scale;
            if ( new_timer <= 6.0f )
                new_timer = 6.0f;
            swarm->swarm_intermittent_action_timer = (int)new_timer;
            intermittent_action_member = seed_random_range(get_global_random_seed_address(), 0, unit_count);
        }
    }
    else
    {
        swarm->swarm_intermittent_action_timer = swarm->swarm_intermittent_action_timer - 1;
    }

    if ( swarm->unit_count <= 0 )
        return;

    /* Steering / timing constants (angles in radians, timers in seconds pre-scale). */
    const float neg_half_pi = -1.5707964f;
    const float neg_one = -1.0f;
    const float min_facing_dot = -0.89999998f;
    const float neg_pi = -3.1415927f;
    const float wander_far_scale = 2.5f;
    const float flee_pause_max = 1.8f;
    const float flee_pause_min = 0.60000002f;
    const float turn_jitter = 0.020943951f;
    const float neg_turn_jitter = -0.020943951f;
    const float wander_angle_decay = -0.06666667f;
    const float wander_move_max = 4.0f;
    const float wander_pause_max = 2.8f;
    const float wander_pause_min = 2.0f;

    int member_slot = 0;

    while ( 1 )
    {
        int move_target_index = -1;
        int movement_mode = _swarm_movement_none;
        int16_t aiming_speed = 1;
        int unit_index = swarm->unit_indices[member_slot];
        char animation_state = _unit_animation_state_in_combat;
        char facing_aligned = 0;
        char target_in_melee = 0;
        /* recovered: (char*)swarm_component_data->data + ((idx<<6)&0x3FFFC0) -> DATA_ARRAY_ELEMENT (stride 64 = swarm_component datum_size) */
        swarm_component_datum *component = DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum,
                swarm->component_indices[member_slot]);
        char have_direction = 0;
        char should_jump = 0;
        int biped_airborne = 0;

        biped_datum *biped = (biped_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

        real_vector3d movement_vector;
        real_vector3d up_reference;

        /* Default up reference = the object's up vector; for a supported biped use the ground-plane normal. */
        up_reference.n[0] = biped->object.up.n[0];
        up_reference.n[1] = biped->object.up.n[1];
        up_reference.n[2] = biped->object.up.n[2];
        if ( biped->object.type == object_type_biped )
        {
            if ( biped->biped.support_surface_index != -1 )
            {
                up_reference.n[0] = biped->biped.ground_plane.normal.n[0];
                up_reference.n[1] = biped->biped.ground_plane.normal.n[1];
                up_reference.n[2] = biped->biped.ground_plane.normal.n[2];
            }
            biped_airborne = biped->biped.flags & (1u << _biped_airborne_bit);
        }

        /* Combat target selection: only when the actor is committed to combat. */
        prop_datum *target_prop;
        if ( actor->state.combat_status < _actor_combat_status_definite )
        {
            target_prop = 0;
            component->combat_target_prop_index = -1;
        }
        else
        {
            float combat_range = variant->ranged_combat.combat_range_upper_bound;
            float best_weight = 0.0f;
            float best_distance = 0.0f;
            prop_datum *best_prop = 0;
            int best_prop_index = -1;

            prop_iterator prop_iter;
            prop_iterator_new(&prop_iter, actor_index);
            for ( prop_datum *prop = prop_iterator_next(&prop_iter); prop; prop = prop_iterator_next(&prop_iter) )
            {
                if ( prop->target_weight > 0.0f )
                {
                    float weight = 0.0f;
                    float delta_x = component->position.n[0] - prop->body_position.n[0];
                    float delta_y = component->position.n[1] - prop->body_position.n[1];
                    float delta_z = component->position.n[2] - prop->body_position.n[2];
                    float distance = __fsqrts(delta_x * delta_x + (delta_z * delta_z + delta_y * delta_y));
                    if ( distance < combat_range )
                        weight = (1.0f - distance / combat_range) * 10.0f;
                    int prop_state = prop->state;
                    if ( prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged )
                    {
                        /* DEVIATION: current prop index is prop_iter.index (disasm reads offset 0), not the
                         * decompiler's HIDWORD(v166). Sticky bonus if it is the member's current target. */
                        weight = prop_iter.index == component->combat_target_prop_index
                               ? weight + 7.0f
                               : weight + 5.0f;
                        if ( !prop->child_units_attached )
                            weight += 5.0f;
                    }
                    if ( weight > best_weight )
                    {
                        best_weight = weight;
                        best_prop = prop;
                        best_distance = distance;
                        best_prop_index = prop_iter.index;
                    }
                }
            }
            component->combat_target_prop_index = best_prop_index;
            if ( best_prop_index != -1 && best_distance < variant->ranged_combat.melee_range )
            {
                int best_state = best_prop->state;
                if ( best_state >= _prop_state_becoming_unacknowledged && best_state <= _prop_state_acknowledged )
                    target_in_melee = 1;
            }
            target_prop = best_prop;
        }

        /* Map the actor's current action to a swarm movement mode + animation grip state. */
        switch ( actor->state.action )
        {
            case actor_action_sleep:
                movement_mode = _swarm_movement_none;
                animation_state = _unit_animation_state_asleep;
                break;
            case actor_action_alert:
                movement_mode = _swarm_movement_wander_noncombat;
                animation_state = _unit_animation_state_alert;
                break;
            case actor_action_flee:
                animation_state = actor->state.action_data.___u0.flee.panic_type <= _actor_panic_none
                        ? _unit_animation_state_in_combat
                        : _unit_animation_state_flee;
                if ( actor->state.action_data.___u0.flee.flee_prop_index != -1 )
                {
                    movement_mode = _swarm_movement_away_from_prop;
                    move_target_index = actor->state.action_data.___u0.flee.flee_prop_index;
                }
                break;
            case actor_action_guard:
                movement_mode = _swarm_movement_wander_idle;
                animation_state = _unit_animation_state_alert;
                break;
            case actor_action_search:
                /* actor->target.target_prop_index @0x270 (actor_target_data, fully modelled). */
                if ( actor->state.action_data.___u0.flee.flee_firing_position_index || actor->target.target_prop_index == -1 )
                {
                    movement_mode = _swarm_movement_wander_combat;
                    animation_state = _unit_animation_state_in_combat;
                }
                else
                {
                    movement_mode = _swarm_movement_towards_prop;
                    move_target_index = actor->target.target_prop_index;
                    animation_state = _unit_animation_state_in_combat;
                }
                break;
            case actor_action_charge:
            case actor_action_obey:
                animation_state = _unit_animation_state_in_combat;
                if ( actor->state.action == actor_action_obey
                  && (component->flags & (1u << _swarm_component_obey_direct_bit)) != 0 )
                {
                    movement_mode = _swarm_movement_obey;
                }
                else if ( component->combat_target_prop_index == -1 )
                {
                    movement_mode = _swarm_movement_wander_combat;
                }
                else
                {
                    int attack_delay = component->attack_delay_ticks;
                    move_target_index = component->combat_target_prop_index;
                    aiming_speed = 0;
                    movement_mode = attack_delay == 0 ? _swarm_movement_towards_prop : _swarm_movement_away_from_prop;
                }
                break;
            default:
                break;
        }

        /* --- Attach / detach to a host unit ------------------------------------------------------ */
        char should_detach = 0;
        if ( biped->object.parent_object_index == -1 )
        {
            int attack_delay = component->attack_delay_ticks;
            component->attached_to_unit_ticks = 0;
            if ( attack_delay )
                component->attack_delay_ticks = attack_delay - 1;
            goto after_parent;
        }

        unit_datum *host = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped->object.parent_object_index)->datum);
        int host_infection_active = (host->object.damage_flags & (1u << _object_dead_bit)) != 0;
        if ( component->attached_to_unit_ticks != 255 )
            ++component->attached_to_unit_ticks;
        int host_alive = host_infection_active;

        if ( !host_infection_active )
        {
            /* Host is no longer a live infection carrier: detach once we have been attached long enough
             * (unless the host is a still-active biped whose tag forbids release). */
            unit_definition *host_definition = TAG_GET(unit_definition, host->definition_index);
            /* +380 = unit_definition.unit.flags; bit 15 name is Guerilla convention (see unit_definition_flags.h) */
            if ( (host->object.type == object_type_biped
                  && (host_definition->unit.flags & (1u << _unit_definition_melee_attackers_cannot_attach_bit)) == 0)
              || component->attached_to_unit_ticks <= 0x2D )
            {
                goto apply_detach;
            }
            component->attack_delay_ticks = 45;
            should_detach = 1;
            goto apply_detach;
        }

        if ( host->unit.time_of_death != -1 && host->unit.time_of_death + 75 < game_time_get() )
        {
            if ( target_prop && target_prop->unit_index != biped->object.parent_object_index )
            {
                int target_state = target_prop->state;
                if ( target_state >= _prop_state_becoming_unacknowledged && target_state <= _prop_state_acknowledged )
                    should_detach = 1;
            }
        }

apply_detach:
        if ( should_detach )
        {
            unit_detach_from_parent(unit_index);
            component->flags &= ~((1u << _swarm_component_melee_engaged_bit)
                                | (1u << _swarm_component_attached_bit));
        }
        else
        {
            component->flags |= (1u << _swarm_component_attached_bit);
            if ( host_alive )
                component->flags &= ~(1u << _swarm_component_melee_engaged_bit);
            else
                component->flags |= (1u << _swarm_component_melee_engaged_bit);
        }

after_parent:
        /* Members not currently parented compute a movement direction; parented ones just fall through
         * with whatever movement_vector holds (they are carried by the host). */
        if ( biped->object.parent_object_index != -1 )
            goto build_packet;

        if ( biped_airborne )
        {
            /* In the air and un-hosted: clear the attached flag and skip active steering. */
            component->flags &= ~(1u << _swarm_component_attached_bit);
            goto build_packet;
        }

        if ( component->ground_ticks != 255 )
            ++component->ground_ticks;

        {
            char wander_flags = component->flags & ~((1u << _swarm_component_melee_engaged_bit)
                                                   | (1u << _swarm_component_attached_bit));
            component->flags &= ~((1u << _swarm_component_melee_engaged_bit)
                                | (1u << _swarm_component_attached_bit));
            if ( movement_mode < _swarm_movement_wander_noncombat || movement_mode > _swarm_movement_obey )
                goto normalize;   /* idle (mode 0) or out of range: no steering direction */

            switch ( movement_mode )
            {
                case _swarm_movement_wander_noncombat:
                case _swarm_movement_wander_idle:
                case _swarm_movement_wander_combat:
                    /* Wander: drift toward the swarm centre, re-choosing a heading on move/pause cycles. */
                    if ( (wander_flags & (1u << _swarm_component_wander_valid_bit)) == 0 )
                    {
                        component->___u9.wander.pause_ticks = 0;
                        component->___u9.wander.move_ticks = 0;
                        component->___u9.wander.vector.n[0] = 0.0f;
                        component->___u9.wander.vector.n[1] = 0.0f;
                        component->___u9.wander.vector.n[2] = 0.0f;
                        component->___u9.wander.angle = 0.0f;
                        /* clears wander_valid|obey_direct then re-latches wander_valid */
                        component->flags = (component->flags & ~((1u << _swarm_component_wander_valid_bit)
                                                               | (1u << _swarm_component_obey_direct_bit)))
                                         | (1u << _swarm_component_wander_valid_bit);
                    }
                    if ( component->___u9.wander.move_ticks )
                    {
                        char remaining_move = component->___u9.wander.move_ticks - 1;
                        component->___u9.wander.move_ticks = remaining_move;
                        if ( remaining_move )
                        {
                            /* Still moving: jitter the heading angle and rotate the wander vector about up. */
                            float decayed = component->___u9.wander.angle * wander_angle_decay;
                            float new_angle = real_seed_random_range(get_global_random_seed_address(),
                                    neg_turn_jitter, turn_jitter) + decayed + component->___u9.wander.angle;
                            component->___u9.wander.angle = new_angle;
                            float cosine = (float)cos(new_angle);
                            float sine = (float)sin(new_angle);
                            rotate_vector_about_axis(&component->___u9.wander.vector, &up_reference, sine, cosine);
                            goto after_wander_vector;
                        }
                        /* Move just finished: seed a pause timer sized by the movement mode. */
                        {
                            float pause_fraction = 1.0f;
                            switch ( movement_mode )
                            {
                                case _swarm_movement_wander_noncombat:
                                    pause_fraction = real_seed_random_range(get_global_random_seed_address(),
                                            wander_move_max, 5.0f);
                                    break;
                                case _swarm_movement_wander_idle:
                                    pause_fraction = real_seed_random_range(get_global_random_seed_address(),
                                            wander_pause_min, wander_pause_max);
                                    break;
                                case _swarm_movement_wander_combat:
                                    pause_fraction = real_seed_random_range(get_global_random_seed_address(),
                                            0.40000001f, 1.0f);
                                    break;
                                default:
                                    break;
                            }
                            int pause_ticks = (int)(pause_fraction * action_duration_scale);
                            char pause_clamped = pause_ticks;
                            if ( (int16_t)pause_ticks > 255 )
                                pause_clamped = -1;
                            component->___u9.wander.pause_ticks = pause_clamped;
                        }
                        goto after_wander_vector;
                    }
                    if ( component->___u9.wander.pause_ticks )
                        --component->___u9.wander.pause_ticks;
                    if ( component->___u9.wander.pause_ticks )
                        goto after_wander_vector;

                    /* Pause finished: seed a new move timer and pick a fresh heading. */
                    {
                        float move_fraction = 1.0f;
                        if ( movement_mode == _swarm_movement_wander_noncombat )
                            move_fraction = real_seed_random_range(get_global_random_seed_address(),
                                    1.0f, wander_far_scale);
                        else if ( movement_mode > _swarm_movement_wander_noncombat
                               && movement_mode <= _swarm_movement_wander_combat )
                            move_fraction = real_seed_random_range(get_global_random_seed_address(),
                                    flee_pause_min, flee_pause_max);
                        int move_ticks = (int)(move_fraction * action_duration_scale);
                        char move_clamped = move_ticks;
                        if ( (int16_t)move_ticks > 255 )
                            move_clamped = -1;
                        component->___u9.wander.move_ticks = move_clamped;
                    }

                    /* Heading toward the swarm centre (or a random direction when already there). */
                    {
                        real_vector3d to_center;
                        to_center.n[1] = swarm->swarm_center.n[1] - component->position.n[1];
                        to_center.n[2] = swarm->swarm_center.n[2] - component->position.n[2];
                        to_center.n[0] = swarm->swarm_center.n[0] - component->position.n[0];
                        float center_distance_sq = to_center.n[0] * to_center.n[0]
                                + (to_center.n[2] * to_center.n[2] + to_center.n[1] * to_center.n[1]);
                        float heading_angle;
                        float heading_z;
                        if ( center_distance_sq >= 0.25f )
                        {
                            float angle_range = 0.5f / __fsqrts(center_distance_sq) * PI;
                            heading_angle = real_seed_random_range(get_global_random_seed_address(),
                                    -angle_range, angle_range);
                            component->___u9.wander.vector.n[0] = to_center.n[0];
                            component->___u9.wander.vector.n[1] = to_center.n[1];
                            heading_z = to_center.n[2];
                        }
                        else
                        {
                            heading_angle = real_seed_random_range(get_global_random_seed_address(),
                                    neg_pi, PI);
                            component->___u9.wander.vector.n[0] = biped->object.forward.n[0];
                            component->___u9.wander.vector.n[1] = biped->object.forward.n[1];
                            heading_z = biped->object.forward.n[2];
                        }
                        component->___u9.wander.vector.n[2] = heading_z;
                        float cosine = (float)cos(heading_angle);
                        float sine = (float)sin(heading_angle);
                        rotate_vector_about_axis(&component->___u9.wander.vector, &up_reference, sine, cosine);
                        component->___u9.wander.angle = 0.0f;
                    }
                    goto after_wander_vector;

                case _swarm_movement_towards_prop:
                case _swarm_movement_away_from_prop:
                {
                    /* Pursue (mode 4) or retreat from (mode 5) the target prop's body position. */
                    prop_datum *pursuit_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, move_target_index);
                    movement_vector.n[0] = pursuit_prop->body_position.n[0] - component->position.n[0];
                    movement_vector.n[1] = pursuit_prop->body_position.n[1] - component->position.n[1];
                    movement_vector.n[2] = pursuit_prop->body_position.n[2] - component->position.n[2];
                    if ( movement_mode == _swarm_movement_away_from_prop )
                    {
                        movement_vector.n[0] = -movement_vector.n[0];
                        movement_vector.n[1] = -movement_vector.n[1];
                        movement_vector.n[2] = -movement_vector.n[2];
                    }
                    have_direction = 1;
                    goto normalize;
                }

                default:
                    break;
            }

            /* movement mode 6 (obey / direct-movement) falls through here. Steer from the obey direct-movement
             * work-area of the swarm_component_datum union: the mode selector is directmovement.facing and the
             * steering direction is directmovement.vector. How that vector is used (cross-product vs. direct)
             * depends on the mode value; the storage accessed is uniformly the directmovement arm. */
            if ( (component->___u9.obey.simple_control_flags & (1u << _obey_simple_control_direct_movement_bit)) == 0 )
                goto obey_direct;
            {
                /* recovered: *(u16*)(component+0x24) -> obey.___u5.directmovement.facing; *(float*)(component+0x28/0x2C/0x30) -> obey.___u5.directmovement.vector.n[0..2] */
                /* facing values 0-3 select forward / negated / cross / negated-cross of the direct vector;
                 * no enum exists for them (the obey atom_modifier vocabulary, also takes 10 and -1). */
                unsigned int obey_mode = (uint16_t)component->___u9.obey.___u5.directmovement.facing;
                have_direction = 1;
                if ( obey_mode >= 2 && obey_mode <= 3 )
                {
                    float obey_x = component->___u9.obey.___u5.directmovement.vector.n[0];
                    float obey_y = component->___u9.obey.___u5.directmovement.vector.n[1];
                    float obey_z = component->___u9.obey.___u5.directmovement.vector.n[2];
                    movement_vector.n[0] = up_reference.n[1] * obey_z - up_reference.n[2] * obey_y;
                    movement_vector.n[1] = up_reference.n[2] * obey_x - obey_z * up_reference.n[0];
                    movement_vector.n[2] = up_reference.n[0] * obey_y - up_reference.n[1] * obey_x;
                    if ( (uint16_t)component->___u9.obey.___u5.directmovement.facing == 3 )
                    {
                        movement_vector.n[0] = -movement_vector.n[0];
                        movement_vector.n[1] = -movement_vector.n[1];
                        movement_vector.n[2] = -movement_vector.n[2];
                    }
                    goto lunge_check;
                }
                movement_vector.n[0] = component->___u9.obey.___u5.directmovement.vector.n[0];
                movement_vector.n[1] = component->___u9.obey.___u5.directmovement.vector.n[1];
                movement_vector.n[2] = component->___u9.obey.___u5.directmovement.vector.n[2];
                if ( (uint16_t)component->___u9.obey.___u5.directmovement.facing == 1 )
                {
                    movement_vector.n[0] = -movement_vector.n[0];
                    movement_vector.n[1] = -movement_vector.n[1];
                    movement_vector.n[2] = -movement_vector.n[2];
                }
            }
obey_direct:
            /* obey direct-movement bit clear (or sub-mode 0): keep the current movement_vector as-is. */
            ;
lunge_check:
            if ( (component->___u9.obey.simple_control_flags & (1u << _obey_simple_control_jump_bit)) != 0 )
            {
                /* Lunge/jump obey state: latch the busy flag once, then steer along object forward. */
                /* recovered: *(u16*)(component+0x24) -> obey.___u5.directmovement.facing */
                if ( (component->___u9.obey.simple_control_flags & (1u << _obey_simple_control_jump_begun_bit)) == 0
                  && !(uint16_t)component->___u9.obey.___u5.directmovement.facing )
                {
                    if ( !unit_is_busy(unit_index) )
                    {
                        component->flags |= (1u << _swarm_component_jump_pending_bit);
                        component->___u9.obey.simple_control_flags |= (1u << _obey_simple_control_jump_begun_bit);
                    }
                }
                have_direction = 1;
                movement_vector.n[0] = biped->object.forward.n[0];
                movement_vector.n[1] = biped->object.forward.n[1];
                movement_vector.n[2] = biped->object.forward.n[2];
            }
            goto normalize;

after_wander_vector:
            if ( component->___u9.wander.move_ticks )
            {
                have_direction = 1;
                movement_vector.n[0] = component->___u9.wander.vector.n[0];
                movement_vector.n[1] = component->___u9.wander.vector.n[1];
                movement_vector.n[2] = component->___u9.wander.vector.n[2];
            }
            goto normalize;
        }

normalize:
        /* --- Normalise the movement direction and build an orthonormal facing frame ------------- */
        if ( !have_direction )
            goto build_packet;

        {
            /* Scalar shadows of the (normalised) movement direction — the decompiler keeps these live so
             * later cross-products read the pre-orthogonalisation values, not the overwritten struct. */
            float mv_x = movement_vector.n[0];
            float mv_y = movement_vector.n[1];
            float mv_z = movement_vector.n[2];
            float length = __fsqrts(mv_x * mv_x + (mv_z * mv_z + mv_y * mv_y));
            if ( __fabs(length) >= 0.000099999997f )
            {
                float inv_length = 1.0f / length;
                mv_x = inv_length * mv_x;
                movement_vector.n[0] = mv_x;
                mv_y = mv_y * inv_length;
                movement_vector.n[1] = mv_y;
                mv_z = mv_z * inv_length;
                movement_vector.n[2] = mv_z;
            }

            float facing_dot = up_reference.n[0] * mv_x + (up_reference.n[2] * mv_z + up_reference.n[1] * mv_y);
            if ( facing_dot > 0.89999998f )
                facing_aligned = 1;

            if ( facing_dot >= min_facing_dot )
            {
                /* Gram-Schmidt: project movement perpendicular to up, giving a stable forward tangent. */
                float cross_x = up_reference.n[1] * mv_z - up_reference.n[2] * mv_y;
                float cross_y = up_reference.n[2] * mv_x - mv_z * up_reference.n[0];
                float tangent_y = (mv_y * up_reference.n[0] - up_reference.n[1] * mv_x) * up_reference.n[0]
                        - up_reference.n[2] * cross_x;
                movement_vector.n[1] = tangent_y;
                movement_vector.n[2] = up_reference.n[1] * cross_x - cross_y * up_reference.n[0];
                float tangent_x = cross_y * up_reference.n[2]
                        - (mv_y * up_reference.n[0] - up_reference.n[1] * mv_x) * up_reference.n[1];
                movement_vector.n[0] = tangent_x;
                float tangent_len = __fsqrts(tangent_x * tangent_x
                        + (movement_vector.n[2] * movement_vector.n[2] + tangent_y * tangent_y));
                if ( __fabs(tangent_len) >= 0.000099999997f )
                {
                    float inv_tangent = 1.0f / tangent_len;
                    movement_vector.n[0] = inv_tangent * tangent_x;
                    movement_vector.n[1] = tangent_y * inv_tangent;
                    movement_vector.n[2] = (up_reference.n[1] * cross_x - cross_y * up_reference.n[0]) * inv_tangent;
                    if ( tangent_len != 0.0f )
                        goto have_basis;
                }
                movement_vector.n[1] = biped->object.forward.n[1];
                movement_vector.n[2] = biped->object.forward.n[2];
                movement_vector.n[0] = biped->object.forward.n[0];
            }
            else
            {
                movement_vector.n[0] = biped->object.forward.n[0];
                movement_vector.n[1] = biped->object.forward.n[1];
                movement_vector.n[2] = biped->object.forward.n[2];
            }
        }

have_basis:
        {
            float basis_x = movement_vector.n[0];
            float basis_y = movement_vector.n[1];
            float basis_z = movement_vector.n[2];

            /* --- Neighbour avoidance (skipped in obey mode 6): nudge the heading away from nearby
             * swarm members that lie ahead of a short probe point. ------------------------------- */
            if ( movement_mode != _swarm_movement_obey )
            {
                int16_t member_count = swarm->unit_count;
                float avoidance_turn = 0.0f;
                if ( member_count > 0 )
                {
                    int other_slot = 0;
                    do
                    {
                        if ( other_slot != member_slot )
                        {
                            /* recovered: (char*)swarm_component_data->data + ((idx<<6)&0x3FFFC0) -> DATA_ARRAY_ELEMENT (stride 64 = swarm_component datum_size) */
                            swarm_component_datum *other = DATA_ARRAY_ELEMENT(swarm_component_data,
                                    swarm_component_datum, swarm->component_indices[other_slot]);
                            float ox = other->position.n[0] - (component->position.n[0] - basis_x * 0.2f);
                            float oy = other->position.n[1] - (component->position.n[1] - basis_y * 0.2f);
                            float oz = other->position.n[2] - (component->position.n[2] - basis_z * 0.2f);
                            float other_dist_sq = ox * ox + (oz * oz + oy * oy);
                            if ( other_dist_sq < 0.64000005f )
                            {
                                float forward_dot = (ox * basis_x + (oz * basis_z + oy * basis_y))
                                        / __fsqrts(other_dist_sq);
                                if ( forward_dot > 0.5f )
                                {
                                    /* Sign from the side the neighbour is on (dot with up x basis). */
                                    float side = ox * (up_reference.n[1] * basis_z - up_reference.n[2] * basis_y)
                                            + (oz * (basis_y * up_reference.n[0] - up_reference.n[1] * basis_x)
                                             + oy * (up_reference.n[2] * basis_x - basis_z * up_reference.n[0]));
                                    if ( side <= 0.0f )
                                        avoidance_turn = (forward_dot - 0.5f) * 0.5f + avoidance_turn;
                                    else
                                        avoidance_turn = -((forward_dot - 0.5f) * 0.5f - avoidance_turn);
                                }
                            }
                        }
                        other_slot = (int16_t)(other_slot + 1);
                    }
                    while ( other_slot < member_count );

                    if ( avoidance_turn != 0.0f )
                    {
                        float turn_angle;
                        if ( avoidance_turn <= 1.0f )
                        {
                            if ( avoidance_turn >= neg_one )
                                turn_angle = avoidance_turn * 1.5707964f;
                            else
                                turn_angle = neg_half_pi;
                        }
                        else
                        {
                            turn_angle = 1.5707964f;
                        }
                        float cosine = (float)cos(turn_angle);
                        float sine = (float)sin(turn_angle);
                        rotate_vector_about_axis(&movement_vector, &up_reference, sine, cosine);
                        basis_x = movement_vector.n[0];
                        basis_y = movement_vector.n[1];
                        basis_z = movement_vector.n[2];
                    }
                }
            }

            /* Final facing = up x basis (normalised), reprojected to lie in the up plane. */
            float right_x = up_reference.n[2] * basis_x - basis_z * up_reference.n[0];
            float right_y = up_reference.n[1] * basis_z - up_reference.n[2] * basis_y;
            float right_z = basis_y * up_reference.n[0] - up_reference.n[1] * basis_x;
            float right_len = __fsqrts(right_y * right_y + (right_z * right_z + right_x * right_x));
            float inv_right;
            float norm_right_y;
            float norm_right_x;
            if ( __fabs(right_len) < 0.000099999997f
              || (norm_right_y = 1.0f / right_len * right_y, norm_right_x = right_x / right_len, right_len == 0.0f) )
            {
                movement_vector.n[0] = biped->object.forward.n[0];
                movement_vector.n[1] = biped->object.forward.n[1];
                movement_vector.n[2] = biped->object.forward.n[2];
            }
            else
            {
                inv_right = 1.0f / right_len;
                movement_vector.n[0] = norm_right_x * up_reference.n[2] - right_z * inv_right * up_reference.n[1];
                movement_vector.n[1] = right_z * inv_right * up_reference.n[0] - up_reference.n[2] * norm_right_y;
                movement_vector.n[2] = up_reference.n[1] * norm_right_y - norm_right_x * up_reference.n[0];
            }
        }

build_packet:
        /* Decide whether to jump/lunge this tick (bit 1 = _unit_control_jump_bit, verified via
         * unit_control -> unit.control_flags -> biped_update_jumping). */
        if ( (component->flags & (1u << _swarm_component_jump_pending_bit)) != 0
          || (have_direction && component->ground_ticks >= 0x2D
              && (member_slot == intermittent_action_member || target_in_melee || facing_aligned)) )
        {
            should_jump = 1;
        }

        /* Update the biped's melee state and record the looking target on the biped. */
        int16_t component_flags = component->flags;
        if ( (component_flags & (1u << _swarm_component_attached_bit)) != 0 )
        {
            if ( (component_flags & (1u << _swarm_component_melee_engaged_bit)) == 0 )
            {
                biped->unit.melee_attack_state = _unit_melee_attack_state_none;
                goto emit_control;
            }
            biped->unit.melee_attack_state = _unit_melee_attack_state_continuous;
        }
        else
        {
            int16_t updated_flags;
            if ( !target_in_melee || component->attack_delay_ticks )
                updated_flags = component->flags & ~(1u << _swarm_component_melee_engaged_bit);
            else
                updated_flags = component->flags | (1u << _swarm_component_melee_engaged_bit);
            component->flags = updated_flags;
            if ( (updated_flags & (1u << _swarm_component_melee_engaged_bit)) == 0 )
            {
                biped->unit.melee_attack_state = _unit_melee_attack_state_none;
                goto emit_control;
            }
            biped->unit.melee_attack_state = _unit_melee_attack_state_impact;
            int looking_target = -1;
            if ( target_prop )
                looking_target = target_prop->unit_index;
            biped->biped.impact_target_object_index = looking_target;
        }

emit_control:
        {
            unit_control_data control;
            memset(&control, 0, sizeof(control));
            control.primary_trigger = 0.0f;
            control.control_flags = should_jump ? (1u << _unit_control_jump_bit) : 0;
            control.weapon_index = -1;
            control.grenade_index = -1;
            control.zoom_level = -1;
            control.animation_state = animation_state;
            control.aiming_speed = aiming_speed;

            float dir_x, dir_y, dir_z;
            if ( have_direction )
            {
                dir_x = movement_vector.n[0];
                control.throttle.n[0] = 1.0f;
                dir_y = movement_vector.n[1];
                control.throttle.n[1] = 0.0f;
                dir_z = movement_vector.n[2];
                control.throttle.n[2] = 0.0f;
            }
            else
            {
                dir_x = biped->object.forward.n[0];
                dir_y = biped->object.forward.n[1];
                dir_z = biped->object.forward.n[2];
                control.throttle.n[0] = global_zero_vector3d->n[0];
                control.throttle.n[1] = global_zero_vector3d->n[1];
                control.throttle.n[2] = global_zero_vector3d->n[2];
            }
            control.facing_vector.n[0] = dir_x;
            control.facing_vector.n[2] = dir_z;
            control.facing_vector.n[1] = dir_y;
            control.aiming_vector.n[0] = dir_x;
            control.aiming_vector.n[1] = dir_y;
            control.aiming_vector.n[2] = dir_z;
            control.looking_vector.n[0] = dir_x;
            control.looking_vector.n[1] = dir_y;
            control.looking_vector.n[2] = dir_z;
            unit_control(unit_index, &control, -1);
        }

        member_slot = (int16_t)(member_slot + 1);
        if ( member_slot >= swarm->unit_count )
            return;
    }
}
