/* actor_move_halt @0x837CB908 — stop an actor's current movement. If the actor is in movement state 4
 * If the actor is a mode-4 vehicle driver (input.vehicle_driver_type == 4) and is currently moving, it
 * re-issues a move to its own body position to settle in place. Otherwise it clears the current path target (+952 = -1), and unless already in path state 1,
 * marks a repath (+1024) and copies the 6-dword pending-goal block (+1024..) over the active goal block
 * (+1132..), then refreshes the actor's path. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/path_state.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_vehicle_driver_type.h"

#include "headers/real_point3d.h"
extern uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index);
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);

uint8_t actor_move_halt(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying && actor->control.moving )
        return actor_move_to_point(actor_index, &actor->input.position.body_position,
                                   actor->input.pathfinding_surface_index, -1);

    int path_state = actor->control.path.destination_orders.destination_type;
    actor->firing_positions.current_position_index = -1;  /* recovered: *((_WORD*)actor+476) -> +952 */

    if ( path_state != 1 )
    {
        /* recovered: *((_WORD *)actor + 512) -> orders.move.destination.destination_type (byte 1024) */
        actor->orders.move.destination.destination_type = _destination_halt;
        actor->control.path.destination_orders = actor->orders.move.destination;
    }

    return actor_path_refresh(actor_index, 1u, nullptr);
}
