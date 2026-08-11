/* actor_move_to_prop @0x837CB7D0 — request an actor move toward a prop (a unit it's aware of), within
 * accept_radius. Clears the current_position_index and un-dormants the actor.
 * If this exact (prop_index, accept_radius) request is already the actor's current one (compared via
 * cached fields in control.path.destination_orders against {5, prop_index}, and the cached radius),
 * it's a no-op refresh: either report success immediately (when not on this actor's scheduled
 * timeslice, or control.path.refreshed_this_tick is set) or just re-run the path refresh. Otherwise,
 * it stores the new request into orders.move.destination (type=5, prop_index, accept_radius, and the
 * prop's vehicle_index — or unit_index if it has no vehicle — at ignore_target_object_index), snapshots
 * that whole destination block into control.path.destination_orders it compares against next time, and
 * fully refreshes the path (rebuild mode). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


#include "headers/path_state.h"
extern void actor_set_dormant(int actor_index, uint8_t dormant);
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);

uint8_t actor_move_to_prop(int actor_index, unsigned int prop_index, float accept_radius)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->firing_positions.current_position_index = -1;
    actor_set_dormant(actor_index, 0);

    uint16_t cached_state = actor->control.path.destination_orders.destination_type;
    int cached_prop_index = actor->control.path.destination_orders.___u3.prop.prop_index;
    /* decompiler wrote .vector.n[1] (a raw-view alias); the prop-destination arm names this same
     * offset (union+0x04) as accept_radius — identical layout. */
    float cached_accept_radius = actor->control.path.destination_orders.___u3.prop.accept_radius;

    if ( cached_state == _destination_prop && (unsigned int)cached_prop_index == prop_index && cached_accept_radius == accept_radius )
    {
        if ( !actor->meta.timeslice || actor->control.path.refreshed_this_tick )
            return 1;
        return actor_path_refresh(actor_index, 0, nullptr);
    }

    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor->orders.move.destination.___u3.prop.accept_radius = accept_radius; /* was .vector.n[1] (same offset) */
    actor->orders.move.destination.___u3.prop.prop_index = prop_index;
    actor->orders.move.destination.destination_type = _destination_prop;
    actor->orders.move.destination.keep_moving = 0;

    int resolved_index = prop->vehicle_index;
    if ( resolved_index == -1 )
        resolved_index = prop->unit_index;
    actor->orders.move.destination.ignore_target_object_index = resolved_index;

    int *source = (int *)&actor->orders.move.destination.destination_type - 1;
    int *dest = (int *)&actor->control.path.destination_orders.destination_type - 1;
    for ( int i = 0; i < 6; i++ )
        *++dest = *++source;

    return actor_path_refresh(actor_index, 1u, nullptr);
}
