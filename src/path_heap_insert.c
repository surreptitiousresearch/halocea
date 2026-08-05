/* path_heap_insert @0x837D2830 — pushes a node onto a path_state's search min-heap (capped at 1024
 * entries) and restores heap order via path_heap_bubble_up. Also inlined directly into path_state_begin. */

#include <stdint.h>
#include "headers/path_state.h"

extern void path_heap_bubble_up(path_state *state, int16_t heap_index);

void path_heap_insert(path_state *state, int16_t node_index, int16_t quantized_cost_estimate)
{
    int16_t heap_index = state->heap_count;
    if (heap_index < 1024)
    {
        state->heap_count = heap_index + 1;
        state->heap[heap_index].node_index = node_index;
        state->heap[heap_index].quantized_cost_estimate = quantized_cost_estimate;
        path_heap_bubble_up(state, heap_index);
    }
}
