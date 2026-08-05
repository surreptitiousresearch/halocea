/* path_heap_bubble_down @0x837D2658 — standard binary min-heap sift-down for the pathfinding node heap
 * (keyed by quantized_cost_estimate): starting from `heap_location`, repeatedly swaps with the smaller of
 * its two children until the heap property holds, keeping each moved node's own heap_location field
 * (path_state.node_list[].heap_location) in sync. */

#include <stdint.h>
#include "headers/path_state.h"

void path_heap_bubble_down(path_state *state, int16_t heap_location)
{
    int16_t node_index = state->heap[heap_location].node_index;
    int16_t cost = state->heap[heap_location].quantized_cost_estimate;

    int16_t target_location;
    while ( 1 )
    {
        target_location = heap_location;
        int16_t target_node_index = node_index;
        int16_t target_cost = cost;

        int16_t first_child = 2 * heap_location;
        for ( int16_t child_offset = 0; child_offset < 2; child_offset++ )
        {
            int16_t child = (int16_t)(first_child + child_offset);
            if ( child >= state->heap_count )
                break;

            if ( state->heap[child].quantized_cost_estimate < target_cost )
            {
                target_location = child;
                target_cost = state->heap[child].quantized_cost_estimate;
                target_node_index = state->heap[child].node_index;
            }
        }

        if ( target_location == heap_location )
            break;

        state->heap[heap_location].node_index = target_node_index;
        state->heap[heap_location].quantized_cost_estimate = target_cost;
        state->node_list[target_node_index].heap_location = heap_location;
        heap_location = target_location;
    }

    state->heap[heap_location].node_index = node_index;
    state->heap[heap_location].quantized_cost_estimate = cost;
    state->node_list[node_index].heap_location = heap_location;
}
