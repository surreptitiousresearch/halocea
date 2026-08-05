/* actor_move_to_point @0x837CB4C8 — orders an actor to path toward a destination point on a given
 * surface. If the actor is already pathing to (essentially) the same point on the same surface, it
 * just refreshes the existing path; otherwise it latches the new path request and rebuilds the path
 * from scratch. Returns the result of actor_path_refresh (or 1 if no refresh was needed). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/path_state.h"
#include "headers/actor_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


#include "headers/path_state.h"
extern void actor_set_dormant(uint16_t actor_index, uint8_t dormant);
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);

uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->firing_positions.current_position_index = -1;
    actor_set_dormant(actor_index, 0);

    float dz = actor->control.path.destination_orders.___u3.raw.point.n[2] - destination->n[2];
    float dx = actor->control.path.destination_orders.___u3.raw.point.n[0] - destination->n[0];
    float dy = actor->control.path.destination_orders.___u3.raw.point.n[1] - destination->n[1];
    if (actor->control.path.destination_orders.destination_type == _destination_raw_location
        && actor->control.path.destination_orders.___u3.raw.surface_index == surface_index
        && (dy * dy + (dx * dx + dz * dz)) <= 0.010000001f)
    {
        if (!actor->meta.timeslice || actor->control.path.refreshed_this_tick)
            return 1;
        return actor_path_refresh(actor_index, 0, nullptr);
    }
    else
    {
        actor->orders.move.destination.destination_type = _destination_raw_location;
        actor->orders.move.destination.keep_moving = 0;
        /* DEVIATION: decompiler word-punned this 12-byte real_point3d copy through the prop_index union arm; plain struct assignment */
        actor->orders.move.destination.___u3.raw.point = *destination;
        actor->orders.move.destination.___u3.raw.surface_index = surface_index;
        actor->orders.move.destination.ignore_target_object_index = ignore_target_object_index;

        /* commit the new request as the active path target (6 dwords: mode, dest xyz, surface, ignore) */
        int *src = (int *)&actor->orders.move.destination;
        int *dst = (int *)&actor->control.path.destination_orders;
        for (int i = 0; i < 6; ++i)
            dst[i] = src[i];

        return actor_path_refresh(actor_index, 1u, nullptr);
    }
}
