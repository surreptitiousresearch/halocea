/* action_guard_setup_postcombat @0x83823898 — set up an actor's post-combat guard state: default to
 * standing guard in place for 120 ticks with no look prop. Bails immediately (returning 0, state_data left
 * at its defaults) if the actor is currently driving a vehicle (vehicle_driver_type == 4). Otherwise, if
 * the actor isn't a vehicle passenger, isn't part of a swarm, and has a script-assigned postcombat prop,
 * sets up looking at that prop for 120 ticks and, for a recognized postcombat_type (6/7/8/9), also derives
 * a guard point from the prop's last-known pathfinding location/surface with a radius keyed by the type
 * (2.0 for 6, 1.0 for 7/8, 1.5 for 9); unrecognized types skip the guard-point derivation. Always returns 1
 * once past the vehicle-driver check. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/guard_state_data.h"
#include "headers/prop_datum.h"
#include "headers/actor_postcombat_type.h"
#include "headers/actor_guard_location_type.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *destination, int value, unsigned int size);


extern void actor_perception_find_prop_pathfinding_location(uint16_t actor_index, uint16_t prop_index);

uint8_t action_guard_setup_postcombat(int actor_index, guard_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(guard_state_data));
    state_data->guard_location_type = _actor_guard_location_current;
    state_data->post_combat = 1;
    state_data->wait_ticks = 120;
    state_data->guard_look_prop_index = -1;

    if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
        return 0;

    if ( !actor->input.vehicle_passenger && !actor->meta.swarm && actor->external_orders.postcombat_prop_index != -1 )
    {
        int prop_index = actor->external_orders.postcombat_prop_index;
        uint8_t recognized_type = 1;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

        state_data->guard_look_until_reached_point = 1;
        state_data->look_ticks = 120;
        state_data->guard_look_prop_index = prop_index;

        if ( (unsigned int)(actor->external_orders.postcombat_type - 6) > 3 )
            recognized_type = 0;
        else if ( actor->external_orders.postcombat_type == actor_postcombat_check_enemy || actor->external_orders.postcombat_type == actor_postcombat_check_friend )
            state_data->___u17.guard_point.radius = 1.0f;
        else if ( actor->external_orders.postcombat_type == actor_postcombat_run_to )
            state_data->___u17.guard_point.radius = 2.0f;
        else
            state_data->___u17.guard_point.radius = 1.5f;

        if ( recognized_type )
        {
            actor_perception_find_prop_pathfinding_location(actor_index, prop_index);
            state_data->guard_location_type = _actor_guard_location_point;
            state_data->___u17.guard_point.position = prop->pathfinding_point;
            state_data->___u17.guard_point.surface_index = prop->pathfinding_surface_index;
        }
    }

    return 1;
}
