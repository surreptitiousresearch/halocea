/* path_heap_bubble_down @0x837D2658 — standard binary min-heap sift-down for the pathfinding node heap
 * (keyed by quantized_cost_estimate): starting from `heap_location`, repeatedly swaps with the smaller of
 * its two children until the heap property holds, keeping each moved node's own heap_location field
 * (path_state.node_list[].heap_location) in sync. */

#include <stdint.h>
#include "headers/path_state.h"

void path_heap_bubble_down(path_state *state, int16_t heap_location)
{
    __int16 node_index = state->heap[heap_location].node_index;
    __int16 cost = state->heap[heap_location].quantized_cost_estimate;

    __int16 target_location;
    while ( 1 )
    {
        target_location = heap_location;
        __int16 target_node_index = node_index;
        __int16 target_cost = cost;

        __int16 first_child = 2 * heap_location;
        for ( __int16 child_offset = 0; child_offset < 2; child_offset++ )
        {
            __int16 child = (__int16)(first_child + child_offset);
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
