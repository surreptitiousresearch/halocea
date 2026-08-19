/* action_guard_setup_find_position @0x838237B8 — resets an actor's "guard" behavior action_data payload and
 * chooses how it will pick a guard spot. Swarm members and actors with input.vehicle_passenger set guard their
 * current direction immediately (guard_location_type 1, no delay). Otherwise: a nonzero `delay_timer` guards
 * the actor's current facing direction (actor->input.facing_vector, the same real_vector3d used by
 * action_guard_setup_current_position) after that many wait_ticks; a zero delay instead requests a fresh guard
 * position be found (find_new_guard_position, guard_location_type 0). Always returns 1. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/guard_state_data.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


uint8_t action_guard_setup_find_position(int actor_index, int16_t delay_timer, guard_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(guard_state_data));

    if (actor->input.vehicle_passenger || actor->meta.swarm)
    {
        state_data->guard_location_type = _actor_guard_location_current;
        state_data->guard_look_prop_index = -1;
        return 1;
    }

    state_data->wait_ticks = delay_timer;

    if (delay_timer)
    {
        state_data->guard_location_type = _actor_guard_location_current;
        state_data->has_guard_direction = 1;
        state_data->guard_direction.n[0] = actor->input.facing_vector.n[0];
        state_data->guard_direction.n[1] = actor->input.facing_vector.n[1];
        state_data->guard_direction.n[2] = actor->input.facing_vector.n[2];
    }
    else
    {
        state_data->find_new_guard_position = 1;
        state_data->guard_location_type = _actor_guard_location_none;
    }

    state_data->guard_look_prop_index = -1;
    return 1;
}
