/* path_heap_pop_cheapest_node @0x837D2768 — pop the cheapest node off a path_state's search min-heap
 * (slot 0 is unused; the root lives at heap[1], matching path_heap_bubble_down's 1-based indexing), mark
 * the node as no longer heap-resident, move the last element to the root and restore heap order.
 * Returns -1 when the heap is empty. */

#include <stdint.h>
#include "headers/path_state.h"

extern void path_heap_bubble_down(path_state *state, int16_t heap_location);

int16_t path_heap_pop_cheapest_node(path_state *state)
{
    if ( state->heap_count <= 1 )
        return -1;

    int16_t node_index = state->heap[1].node_index;
    state->node_list[node_index].heap_location = -1;

    int16_t new_count = state->heap_count - 1;
    state->heap_count = new_count;
    if ( new_count > 1 )
    {
        state->heap[1] = state->heap[new_count];
        path_heap_bubble_down(state, 1);
    }
    return node_index;
}
