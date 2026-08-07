/* action_guard_setup_current_position @0x83823730 — resets an actor's "guard" behavior action_data payload to
 * guard its current facing direction (guard_location_type 1 = "direction", no target point/prop). The guard
 * direction is copied from the actor's own facing vector (actor+372, a real_vector3d — same field referenced
 * by actor_move_update.c/actor_combat_reaim_grenade.c/actor_input_update.c). Always returns 1. */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/guard_state_data.h"
#include "headers/real_vector3d.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


uint8_t action_guard_setup_current_position(uint16_t actor_index, guard_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(guard_state_data));

    state_data->guard_location_type = _actor_guard_location_current;
    state_data->has_guard_direction = 1;
    state_data->guard_direction = actor->input.facing_vector;
    state_data->guard_look_prop_index = -1;

    return 1;
}
