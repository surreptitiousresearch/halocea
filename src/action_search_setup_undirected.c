#include <stdint.h>
#include <string.h>

#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/search_state_data.h"
#include "headers/pursuit_location_type.h"
#include "headers/blam_data_globals.h"


int action_search_setup_undirected(int actor_index, uint8_t tenacious, search_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(search_state_data));
    if ( !actor->meta.swarm )
        return 0;

    state_data->pursuit_location.type = _pursuit_location_undirected;
    actor->state.searching = 1;
    return 1;
}
