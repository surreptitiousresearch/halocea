/* action_search_setup_target @0x83822B48 — initialize a fresh search_state_data block for the actor,
 * bailing out (returning 0, leaving state_data zeroed) if the actor is not allowed to search (byte +352). */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/search_state_data.h"
#include "headers/pursuit_location_type.h"
#include "headers/blam_data_globals.h"


uint8_t action_search_setup_target(int actor_index, uint8_t must_charge, search_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(search_state_data));
    if ( actor->input.vehicle_passenger )
        return 0;

    state_data->charging = must_charge;
    state_data->pursuit_location.type = _pursuit_location_target;
    actor->state.searching = 1;
    return 1;
}
