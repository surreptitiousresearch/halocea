/* path_state_reset @0x837D2518 */
#include <string.h>

#include "headers/path_state.h"

void path_state_reset(path_state *state)
{
    state->node_count = 0;
    state->heap_count = 1;
    memset(state->hash_table, -1, sizeof(state->hash_table));
    state->closest_node_index = -1;
    state->closest_distance = 3.4028235e38f;
    state->closest_cost_estimate = 3.4028235e38f;
}
