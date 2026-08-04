/* path_state_find @0x837D3B90 — resets a path_state's search working set (node/heap counts, hash table,
 * closest-node tracking) and runs the pathfinding search: path_state_begin seeds the open set from the
 * start point, and path_state_traverse runs the search proper (A*-style, given closest_distance/cost seed
 * to FLT_MAX). Returns 0 immediately if path_state_begin fails to seed a starting node. */

#include <stdint.h>
#include <string.h>

#include "headers/path_state.h"

extern uint8_t path_state_begin(path_state *state);
extern uint8_t path_state_traverse(path_state *state);

uint8_t path_state_find(path_state *state)
{
    state->node_count = 0;
    state->heap_count = 1;
    memset(state->hash_table, -1, sizeof(state->hash_table));
    state->closest_node_index = -1;
    state->closest_distance = 3.4028235e38f;
    state->closest_cost_estimate = 3.4028235e38f;

    if (!path_state_begin(state))
        return 0;
    return path_state_traverse(state);
}
