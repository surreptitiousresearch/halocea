/* heap_insert @0x8381D288 — push one step index onto an obstacle_path's binary min-heap (ordered by step
 * total_distance) and sift it up, companion to heap_up/heap_down. Fails (returns 0) when the heap is at its
 * 128-entry capacity. */

#include <stdint.h>
#include "headers/obstacle_path.h"

uint8_t heap_insert(obstacle_path *path, int16_t step_index)
{
    int16_t child_index = path->heap_count;
    if ( child_index >= 128 )
        return 0;

    path->heap_count = child_index + 1;
    path->heap[child_index] = step_index;

    while ( child_index > 0 )
    {
        int16_t parent_index = (int16_t)(child_index - 1) >> 1;
        int16_t child_step_index = path->heap[child_index];
        int16_t parent_step_index = path->heap[parent_index];

        if ( path->steps[parent_step_index].total_distance <= path->steps[child_step_index].total_distance )
            break;

        path->heap[parent_index] = child_step_index;
        path->heap[child_index] = parent_step_index;
        child_index = parent_index;
    }

    return 1;
}
