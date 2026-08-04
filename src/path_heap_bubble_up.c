/* path_heap_bubble_up @0x837D2588 — standard binary min-heap sift-up for a path_state's A* open list,
 * ordered by quantized_cost_estimate. Moves the element at `heap_index` up toward the root while its
 * parent has a larger cost estimate, keeping each moved node's own heap_location back-pointer in sync. */

#include <stdint.h>
#include "headers/path_state.h"

void path_heap_bubble_up(path_state *state, int16_t heap_index)
{
    __int16 node_index = state->heap[heap_index].node_index;
    __int16 quantized_cost_estimate = state->heap[heap_index].quantized_cost_estimate;

    if (heap_index > 1)
    {
        __int16 parent_index;
        do
        {
            parent_index = heap_index >> 1;
            __int16 parent_cost_estimate = state->heap[parent_index].quantized_cost_estimate;
            __int16 parent_node_index = state->heap[parent_index].node_index;

            if (quantized_cost_estimate >= parent_cost_estimate)
                break;

            state->heap[heap_index].node_index = parent_node_index;
            state->heap[heap_index].quantized_cost_estimate = parent_cost_estimate;
            state->node_list[parent_node_index].heap_location = heap_index;
            heap_index = heap_index >> 1;
        } while (parent_index > 1);
    }

    state->heap[heap_index].node_index = node_index;
    state->heap[heap_index].quantized_cost_estimate = quantized_cost_estimate;
    state->node_list[node_index].heap_location = heap_index;
}
