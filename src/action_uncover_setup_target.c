/* action_uncover_setup_target @0x838247C8 — initialize a fresh uncover_state_data block for the actor,
 * bailing out (returning 0, leaving state_data zeroed) if the actor may not uncover (byte +352 or +6). */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/uncover_state_data.h"
#include "headers/pursuit_location_type.h"
#include "headers/blam_data_globals.h"


uint8_t action_uncover_setup_target(int actor_index, uint8_t able_to_search, uncover_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(uncover_state_data));
    if ( actor->input.vehicle_passenger || actor->meta.swarm )
        return 0;

    state_data->able_to_search = able_to_search;
    state_data->pursuit_location.type = _pursuit_location_target;
    return 1;
}
