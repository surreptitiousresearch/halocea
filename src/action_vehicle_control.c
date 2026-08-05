/* action_vehicle_control @0x838214B8 — enter the "in vehicle" action for an actor: if flagged "prefer parked
 * destination" (+200), copies the actor's saved destination point (+216..224) into the vehicle action's
 * saved-destination fields (+1008..1016) and enters sub-state 4; otherwise enters sub-state 3 if the actor
 * has a path, or sub-state 0 if not. Always resets the sub-action progress fields.
 *
 * Fields resolved to named actor_datum members and the DB-named vehicle_state_data arm of action_data
 * (prefer-parked flag +200, saved destination point +216.., vehicle-action destination copy +1008..). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_path_has_path(uint16_t actor_index);

void action_vehicle_control(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action_data.___u0.vehicle.lock_facing )
    {
        actor->orders.look.primary_priority = _primary_priority_exact_facing;
        actor->orders.look.primary_direction.type = _direction_specification_vector;
        /* DEVIATION: decompiler word-punned this 12-byte real_vector3d copy (lwz/stw triple, 0x3F0..0x3F8) through the point arm; type is _direction_specification_vector, use the vector arm */
        actor->orders.look.primary_direction.___u1.vector = actor->state.action_data.___u0.vehicle.destination_facing;
    }
    else if ( actor_path_has_path(actor_index) )
    {
        actor->orders.look.primary_direction.type = _direction_specification_movement;
        actor->orders.look.primary_priority = _primary_priority_facing;
    }
    else
    {
        actor->orders.look.primary_priority = _primary_priority_none;
    }

    actor->orders.look.idle_look_type = _idle_look_combat;
    actor->orders.combat.shoot_at_target = 0;
    actor->orders.move.stationary_crouch = 0;
    actor->orders.move.moving_crouch = 0;
    actor->orders.move.panicked = 0;
    actor->orders.move.dive_into_cover = 0;
    actor->orders.move.emerge_from_cover = 0;
}
