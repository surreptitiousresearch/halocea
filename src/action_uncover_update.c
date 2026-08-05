/* action_uncover_update @0x83824BD8 — per-tick update of an actor's "uncover" action working state (the AI
 * behaviour that decides whether the actor should break from cover). It first (re)computes whether the actor
 * currently wants to uncover: for uncover-mode 1 it uncovers when the character can uncover (definition flag 0x4)
 * and either the definition permits it unconditionally (field +760 != 4) or the target is within 10 units; for
 * uncover-mode 0 it uncovers when the definition flag 0x2 is set, the actor's cover state (+616) is 5, and the
 * target prop is sufficiently threatening (or, when +760 == 4, it packs an uncover code from the cover state and
 * +616 byte). It then advances a "give up" timer (discarding the current firing position after 30 ticks unless a
 * global reset byte +1284 is set), evaluates a "should hold" condition from the current firing position / target
 * prop, runs a hold/decay counter, and finally latches the action-complete flag (+157) when the hold counter
 * expires, the decay counter reaches 360, or (in mode 1) an immediate-finish byte +188 is set.
 *
 * DEVIATION: the actor's uncover working-state is resolved to the DB-named uncover_state_data arm of
 * action_data; remaining state/behaviour fields use named actor_datum members.
 * `-v & ~v >> 31` on the prop's signed field is the branchless `(field > 0)` idiom (so `v14 >= 0` is `field <= 0`);
 * decoded with a comment. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/defensive_crouch_type.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_target_type.h"
#include "headers/uncover_state_data.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);

void action_uncover_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uncover_state_data *uncover_state = &actor->state.action_data.___u0.uncover;
    if ( actor->state.action_data.___u0.uncover.uncover_done )   /* already finished */
        return;

    int16_t uncover_mode = actor->state.action_data.___u0.uncover.pursuit_location.type;
    actor_definition *character_definition = TAG_GET(actor_definition, actor->meta.definition_index);
    uncover_state->sneaking = 0;

    if ( uncover_mode )
    {
        if ( uncover_mode == 1 )
        {
            char want_uncover = 1;
            if ( character_definition->defensive.defensive_crouch_type != _defensive_crouch_any_target )
            {
                if ( (character_definition->flags & (1u << _actor_definition_sneak_uncovering_pursuit_position_bit)) == 0 )
                {
                    want_uncover = 0;
                }
                else
                {
                    float delta_z = (actor->state.action_data.___u0.uncover.pursuit_location.position.z - actor->input.position.body_position.z);
                    float delta_x = (actor->state.action_data.___u0.uncover.pursuit_location.position.x - actor->input.position.body_position.x);
                    float delta_y = (actor->state.action_data.___u0.uncover.pursuit_location.position.y - actor->input.position.body_position.y);
                    if ( ((delta_y * delta_y)
                            + ((delta_x * delta_x) + (delta_z * delta_z))) >= 100.0f )
                        want_uncover = 0;
                }
            }
            if ( want_uncover )
                uncover_state->sneaking = 1;
        }
    }
    else if ( character_definition->defensive.defensive_crouch_type != _defensive_crouch_any_target )
    {
        if ( (character_definition->flags & (1u << _actor_definition_sneak_uncovering_target_bit)) != 0
          && actor->target.target_type == actor_target_uninspected_orphan
          && (DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index))->quantized_distance <= 2 )
        {
            uncover_state->sneaking = 1;
        }
    }
    else
    {
        uncover_state->sneaking = actor->target.target_type - 6
                - ((char)actor->target.target_type - 7 + (actor->target.target_type == actor_target_definite_orphan));
    }

    if ( actor->control.moving )
    {
        actor->state.action_data.___u0.uncover.current_position_hold_timer = 0;
    }
    else
    {
        int give_up_timer = actor->state.action_data.___u0.uncover.current_position_hold_timer + 1;
        actor->state.action_data.___u0.uncover.current_position_hold_timer = give_up_timer;
        if ( !uncover_mode && give_up_timer >= 30 )
            actor_discard_firing_position(actor_index, actor->firing_positions.current_position_index, 0);
    }

    char should_hold = 0;
    unsigned int prop_threat_positive = 0;
    if ( uncover_mode )
    {
        should_hold = actor->state.action_data.___u0.uncover.pursuit_location_inspected == 0;
    }
    else if ( actor->target.target_prop_index != -1 )
    {
        int16_t prop_threat = (DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index))->visibility;
        prop_threat_positive = (unsigned int)(-prop_threat & ~prop_threat) >> 31;   /* branchless (prop_threat > 0) */
        char hold = 0;
        if ( prop_threat <= 0 || actor->target.target_type >= actor_target_uninspected_orphan )   /* -v&~v >= 0  <=>  v <= 0 */
            hold = 1;
        should_hold = hold;
    }

    char keep_uncovering = 1;
    if ( (uint16_t)actor->firing_positions.current_position_index == 0xFFFF )
    {
        keep_uncovering = 0;
        actor->state.action_data.___u0.uncover.uncover_exit_failure = 1;
    }
    else if ( should_hold )
    {
        if ( !actor->input.vehicle_gunner_bombardment && !(uint8_t)prop_threat_positive )
            keep_uncovering = actor->control.moving != 0;
    }
    else
    {
        keep_uncovering = 0;
    }

    if ( keep_uncovering )
    {
        actor->state.action_data.___u0.uncover.uncover_remaining_time = actor->state.action_data.___u0.uncover.uncover_desired_time;
    }
    else
    {
        int hold_counter = actor->state.action_data.___u0.uncover.uncover_remaining_time;
        if ( hold_counter > 0 )
            actor->state.action_data.___u0.uncover.uncover_remaining_time = hold_counter - 1;
        ++actor->state.action_data.___u0.uncover.uncover_exit_persistent_timer;
    }

    char finished = 0;
    if ( !actor->state.action_data.___u0.uncover.uncover_remaining_time || actor->state.action_data.___u0.uncover.uncover_exit_persistent_timer >= 360 )
        finished = 1;
    if ( uncover_mode == 1 && actor->state.action_data.___u0.uncover.pursuit_location_inspected )
        finished = 1;
    actor->state.action_data.___u0.uncover.uncover_done = finished;
}
