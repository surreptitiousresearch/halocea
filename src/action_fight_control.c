/* action_fight_control @0x83826450 — enter the "fight" action state: set the actor's look orders for combat
 * (primary_priority = _primary_priority_aiming (5), primary_direction.type = _direction_specification_target
 * (2), idle_look_type = _idle_look_combat (4)) and save/clear a run of flag bytes at orders.move (preserving
 * emotions.defensive_crouch into orders.move.stationary_crouch). If the actor is not a directional-flying
 * vehicle driver (input.vehicle_driver_type != _actor_vehicle_driver_directional_flying) and is at high
 * awareness (state.combat_status >= _actor_combat_status_clear_los), additionally mark
 * orders.combat.shoot_at_target and escalate the look priority to _primary_priority_locked_aiming (7). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


void action_fight_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    char saved_flag = actor->emotions.defensive_crouch;
    unsigned __int16 vehicle_driver_type = (unsigned __int16)actor->input.vehicle_driver_type;

    actor->orders.look.primary_priority = _primary_priority_aiming;
    actor->orders.look.primary_direction.type = _direction_specification_target;
    actor->orders.look.idle_look_type = _idle_look_combat;
    actor->orders.move.stationary_crouch = saved_flag;
    actor->orders.move.moving_crouch = 0;
    actor->orders.move.panicked = 0;
    actor->orders.move.dive_into_cover = 0;
    actor->orders.move.emerge_from_cover = 0;

    if ( vehicle_driver_type != _actor_vehicle_driver_directional_flying
      && actor->state.combat_status >= _actor_combat_status_clear_los )
    {
        actor->orders.combat.shoot_at_target = 1;
        actor->orders.look.primary_priority = _primary_priority_locked_aiming;
    }
}
