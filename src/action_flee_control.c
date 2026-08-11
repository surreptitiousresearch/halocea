/* action_flee_control @0x83825DC8 — per-tick control for an actor's "flee" action (sibling of
 * action_uncover_control.c / action_wait_control.c). Picks the sub-action state word (+1000) and approach
 * variant (+1004) from the flee timer (+168) and, while the timer is spent, from whether the flee target prop
 * (+624) is currently visible and whether a stored retreat point (+184) exists. It then resets the common
 * sub-action progress scratch (+1020/+1060..+1065), marks the "in dangerous range" flag (+1065) when the timer
 * is in [9,12], and drives movement: with no firing position (+164 == -1) it halts; otherwise, if allowed
 * (+76), it moves to that firing position, caching it on success (+952/+954) or discarding the previously
 * cached position and halting on failure.
 *
 * Fields resolved to named actor_datum members and the DB-named flee_state_data arm of action_data
 * (flee timer +168, target prop index +624, firing position index +164, stored retreat point +184,
 * sub-action state block +1000.., cached firing position +952, and the flag bytes). Prop visibility uses prop_datum.h. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/flee_state_data.h"
#include "headers/actor_panic_type.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


#include "headers/path_state.h"
extern uint8_t actor_move_halt(int actor_index);
extern uint8_t actor_move_to_firing_position(int actor_index, int16_t firing_position_index, path_state *cached_path_state);
extern void actor_discard_firing_position(int actor_index, int16_t firing_position_index, uint8_t temporary);

void action_flee_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    flee_state_data *flee_state = &actor->state.action_data.___u0.flee;

    if ( flee_state->panic_type <= _actor_panic_none )
    {
        uint8_t target_visible = 0;
        if ( actor->target.target_prop_index != -1 )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
            target_visible = prop->visibility > 0;
        }

        if ( target_visible )
        {
            actor->orders.combat.shoot_at_target = 1;
            actor->orders.look.primary_priority = _primary_priority_locked_aiming;
            actor->orders.look.primary_direction.type = _direction_specification_target;
        }
        else if ( flee_state->flee_prop_index == -1 )
        {
            actor->orders.look.primary_priority = _primary_priority_none;
        }
        else
        {
            actor->orders.look.primary_direction.type = _direction_specification_prop;
            actor->orders.look.primary_priority = _primary_priority_facing;
            actor->orders.look.primary_direction.___u1.prop_index = flee_state->flee_prop_index;
        }
    }
    else
    {
        actor->orders.look.primary_direction.type = _direction_specification_movement;
        actor->orders.combat.abort_burst = 1;
        actor->orders.look.primary_priority = _primary_priority_locked_facing;
    }

    actor->orders.look.idle_look_type = _idle_look_combat;
    actor->orders.move.panicked = flee_state->panic_type > _actor_panic_none; /* source emits the (-x & ~x) < 0 signed-positive idiom */

    int panic_type = (uint16_t)flee_state->panic_type;
    char in_dangerous_range =
        (panic_type >= _actor_panic_grenade_attached_to_us && panic_type <= _actor_panic_burning_to_death) ? 1 : 0;
    actor->orders.move.dive_into_cover = 1;
    actor->orders.move.moving_crouch = 0;
    actor->orders.move.emerge_from_cover = 0;
    actor->orders.move.flaming = in_dangerous_range;
    actor->orders.move.stationary_crouch = 1;

    int firing_position_index = (uint16_t)flee_state->flee_firing_position_index;
    if ( firing_position_index == 0xFFFF )
    {
        actor_move_halt(actor_index);
    }
    else if ( actor->meta.timeslice )
    {
        if ( actor_move_to_firing_position(actor_index, firing_position_index, 0) )
        {
            actor->firing_positions.current_position_index = (uint16_t)flee_state->flee_firing_position_index;
            actor->firing_positions.current_position_found_outside_range = flee_state->flee_firing_position_found_randomly;
        }
        else
        {
            int cached_firing_position = (uint16_t)actor->firing_positions.current_position_index; /* DEVIATION: zero-extending lhz, not a pointer pun */
            if ( cached_firing_position != 0xFFFF )
            {
                actor_discard_firing_position(actor_index, cached_firing_position, 0);
                actor_move_halt(actor_index);
                actor->firing_positions.current_position_index = -1;
            }
            flee_state->flee_firing_position_index = -1;
            flee_state->find_new_flee_position = 1;
        }
    }
}
