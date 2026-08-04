/* action_guard_control @0x83824208 — per-tick controller for the guard behaviour. It first decides the two
 * guard sub-flags (actor->orders.move.stationary_crouch = "should guard position", actor->orders.move.moving_crouch = "forced guard") from the unit
 * definition flags, the guard mode (actor word +192) and the alert level (actor word +110). Then, while the
 * actor is active and not deactivated, it either walks to / holds at the guard point (mode 2), moves to the
 * assigned firing position (mode 0), or halts, and on arrival fires the post-combat / suspicion handling
 * (stimulus + AI communication event 7). Finally it builds this tick's look/aim orders (actor->orders.look fields,
 * targeting actor from actor->orders.combat) from the current guard state and sets the idle look type
 * (actor->orders.look.idle_look_type: _idle_look_guarding below certain combat status, _idle_look_combat otherwise).
 *
 * The opaque behaviour payload is accessed via typed struct field accessors (actor_datum *actor). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_postcombat_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_information_type.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
#include "headers/real_point3d.h"
#include "headers/path_state.h"
extern uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern void actor_stimulus_suspicion(uint16_t actor_index, int16_t suspicion_combat_status, int suspicion_timer);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern int action_guard_speak_post_combat(uint16_t actor_index);
extern uint8_t actor_move_to_firing_position(int actor_index, int16_t firing_position_index, path_state *cached_path_state);
extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);

void action_guard_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( (actor_def->flags & (1u << _actor_definition_crouch_when_noncombat_bit)) == 0 || actor->state.combat_status )
    {
        actor->orders.move.moving_crouch = 0;
        if ( actor->state.action_data.___u0.guard.cower )
        {
            if ( actor->state.action_data.___u0.guard.cower_from_retreat )
                actor->orders.move.stationary_crouch = (actor_def->flags & (1u << _actor_definition_crouch_when_hiding_from_unopposable_enemy_bit)) != 0;
            else
                actor->orders.move.stationary_crouch = 1;
        }
        else
        {
            char should_guard = 1;
            if ( (actor_def->flags & (1u << _actor_definition_crouch_when_guarding_bit)) == 0 || actor->state.combat_status <= _actor_combat_status_none )
                should_guard = 0;
            actor->orders.move.stationary_crouch = should_guard;
        }
    }
    else
    {
        actor->orders.move.stationary_crouch = 1;
        actor->orders.move.moving_crouch = 1;
    }

    unsigned __int8 active = actor->meta.timeslice;
    actor->orders.move.panicked = 0;
    actor->orders.move.dive_into_cover = 0;
    actor->orders.move.emerge_from_cover = 0;
    if ( active && !actor->meta.swarm )
    {
        unsigned int guard_mode = (unsigned __int16)actor->state.action_data.___u0.guard.guard_location_type;
        char arrived = 0;
        if ( guard_mode > _actor_guard_location_firing_position )
            goto arrived_check;

        if ( guard_mode != _actor_guard_location_current )
        {
            if ( guard_mode == _actor_guard_location_point )
            {
                float d1 = actor->state.action_data.___u0.guard.___u17.guard_point.position.y - actor->input.position.body_position.y;
                float d2 = actor->state.action_data.___u0.guard.___u17.guard_point.position.z - actor->input.position.body_position.z;
                float d0 = actor->state.action_data.___u0.guard.___u17.guard_point.position.x - actor->input.position.body_position.x;
                float distance_squared = ((d0 * d0) + ((d2 * d2) + (d1 * d1)));
                if ( distance_squared >= (actor->state.action_data.___u0.guard.___u17.guard_point.radius * actor->state.action_data.___u0.guard.___u17.guard_point.radius) )
                    actor_move_to_point(actor_index, &actor->state.action_data.___u0.guard.___u17.guard_point.position, actor->state.action_data.___u0.guard.___u17.guard_point.surface_index, -1);
                else
                    actor_move_halt(actor_index);
                arrived = distance_squared < 9.0f;
                goto arrived_check;
            }

            if ( actor->state.action_data.___u0.guard.guard_location_type )
            {
                int firing_position = (unsigned __int16)actor->state.action_data.___u0.guard.___u17.guard_firing_position_index;
                if ( firing_position != 0xFFFF )
                {
                    actor->firing_positions.current_position_index = firing_position;
                    actor->firing_positions.current_position_found_outside_range = 0;
                    if ( !actor_move_to_firing_position(actor_index, firing_position, 0) )
                    {
                        actor_discard_firing_position(actor_index, actor->state.action_data.___u0.guard.___u17.guard_firing_position_index, 0);
                        actor->firing_positions.current_position_index = -1;
                    }
                }
                arrived = 0;
                if ( !actor->control.path.path.valid )
                {
                    arrived = 1;
                }
                else
                {
                    float d0 = actor->input.position.body_position.y - actor->control.path.path.endpoint.point.y;
                    float d1 = actor->input.position.body_position.z - actor->control.path.path.endpoint.point.z;
                    float d2 = actor->input.position.body_position.x - actor->control.path.path.endpoint.point.x;
                    if ( ((d2 * d2) + ((d1 * d1) + (d0 * d0))) < 9.0f )
                        arrived = 1;
                }
                goto arrived_check;
            }
        }

        actor_move_halt(actor_index);
        arrived = 1;

arrived_check:
        actor->state.action_data.___u0.guard.path_begun = 1;
        if ( arrived )
        {
            if ( actor->state.action_data.___u0.guard.shout_about_dead_friend )
            {
                int prop_index = (unsigned __int16)actor->state.action_data.___u0.guard.shout_dead_friend_prop_index;
                prop_datum *prop = &((prop_datum *)prop_data->data)[prop_index];
                actor->state.action_data.___u0.guard.shout_dead_friend_prop_index = -1;
                actor->state.action_data.___u0.guard.shout_about_dead_friend = 0;
                actor_stimulus_suspicion(actor_index, _actor_combat_status_investigate, 600);
                ai_communication_event(_ai_communication_found_dead_friend, actor->meta.unit_index, prop->unit_index, -1, -1, _ai_information_combat_stimulus, 0);
            }
            if ( actor->state.action_data.___u0.guard.post_combat )
            {
                action_guard_speak_post_combat(actor_index);
                if ( actor->external_orders.postcombat_type == actor_postcombat_shoot_corpse && actor->state.action_data.___u0.guard.guard_location_type == _actor_guard_location_point )
                    actor->state.action_data.___u0.guard.post_combat_shooting = 1;
            }
        }
    }

    if ( actor->state.action_data.___u0.guard.post_combat_shooting )
    {
        actor->orders.combat.shoot_at_target = 1;
        actor->orders.combat.use_manual_target_point = 1;
        actor->orders.look.primary_priority = _primary_priority_locked_aiming;
        actor->orders.look.primary_direction.type = _direction_specification_target;
        actor->output.animation.alignment_vector.__s1.i = (global_up3d->n[0] * (float)0.050000001) + actor->state.action_data.___u0.guard.___u17.guard_point.position.x;
        actor->output.animation.alignment_vector.__s1.j = (global_up3d->n[1] * (float)0.050000001) + actor->state.action_data.___u0.guard.___u17.guard_point.position.y;
        /* DEVIATION: DB types animation.alignment_vector as real_vector2d (8B); the decompiler's
         * .z store lands one float past it. Preserved byte-exact via explicit +8 offset. See escalation. */
        *(float *)((char *)&actor->output.animation.alignment_vector + 8) = (global_up3d->n[2] * (float)0.050000001) + actor->state.action_data.___u0.guard.___u17.guard_point.position.z;
    }
    else if ( actor->state.action_data.___u0.guard.guard_look_prop_index == -1 )
    {
        if ( actor->state.action_data.___u0.guard.has_guard_direction )
        {
            actor->orders.look.primary_direction.type = _direction_specification_vector;
            actor->orders.look.primary_priority = actor->state.action_data.___u0.guard.aim_in_guard_direction == 0 ? _primary_priority_facing : _primary_priority_aiming;
            actor->orders.look.primary_direction.___u1.prop_index = *(int *)&actor->state.action_data.___u0.guard.guard_direction.i;
            *(int *)&actor->orders.look.primary_direction.___u1.point.y = *(int *)&actor->state.action_data.___u0.guard.guard_direction.j;
            *(int *)&actor->orders.look.primary_direction.___u1.point.z = *(int *)&actor->state.action_data.___u0.guard.guard_direction.k;
        }
        else
        {
            int prop_index;
            if ( actor->state.combat_status <= _actor_combat_status_none || (prop_index = actor->target.target_prop_index, prop_index == -1) )
            {
                actor->orders.look.primary_priority = _primary_priority_none;
            }
            else
            {
                actor->orders.look.primary_direction.___u1.prop_index = prop_index;
                actor->orders.look.primary_direction.type = _direction_specification_prop;
                actor->orders.look.primary_priority = _primary_priority_facing;
            }
        }
    }
    else
    {
        actor->orders.look.primary_direction.type = _direction_specification_prop;
        actor->orders.look.primary_priority = _primary_priority_aiming;
        actor->orders.look.primary_direction.___u1.prop_index = actor->state.action_data.___u0.guard.guard_look_prop_index;
    }

    /* deviation: prior comment called this a "look speed" — it is the idle look type
     * (_idle_look_guarding vs _idle_look_combat). */
    __int16 idle_look = _idle_look_combat;
    if ( actor->state.combat_status < _actor_combat_status_certain )
        idle_look = _idle_look_guarding;
    actor->orders.look.idle_look_type = idle_look;
}