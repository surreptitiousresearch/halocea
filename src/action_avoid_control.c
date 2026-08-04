/* action_avoid_control @0x83821374 — sets up an actor's "avoid" maneuver targets and timers. If the actor's
 * threat/danger level (target.target_type) is below 5, picks a mild avoid mode
 * (orders.look.primary_direction.type = _direction_specification_target or _danger, depending on whether
 * danger_zone.danger_type is positive) and sets
 * orders.look.primary_priority = _primary_priority_aiming; otherwise forces the strongest avoid mode
 * (orders.combat.shoot_at_target = 1, orders.look.primary_priority = _primary_priority_locked_aiming,
 * orders.look.primary_direction.type = _direction_specification_target). Then resets shared avoid-path scratch fields
 * (orders.move.dive_into_cover, orders.move.emerge_from_cover, orders.move.stationary_crouch,
 * orders.move.panicked, orders.look.idle_look_type) regardless of which branch was taken,
 * carrying emotions.defensive_crouch through to orders.move.stationary_crouch. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_target_type.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


void action_avoid_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->target.target_type < actor_target_uninspected_orphan )
    {
        if ( actor->danger_zone.danger_type <= actor_danger_zone_none )
            actor->orders.look.primary_direction.type = _direction_specification_target;
        else
            actor->orders.look.primary_direction.type = _direction_specification_danger;
        actor->orders.look.primary_priority = _primary_priority_aiming;
    }
    else
    {
        actor->orders.combat.shoot_at_target = 1;
        actor->orders.look.primary_priority = _primary_priority_locked_aiming;
        actor->orders.look.primary_direction.type = _direction_specification_target;
    }

    char carried = (char)actor->emotions.defensive_crouch;
    actor->orders.move.moving_crouch = 0;
    actor->orders.look.idle_look_type = _idle_look_combat;
    actor->orders.move.panicked = 0;
    actor->orders.move.stationary_crouch = carried;
    actor->orders.move.dive_into_cover = 0;
    actor->orders.move.emerge_from_cover = 0;
}
