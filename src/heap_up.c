/* heap_up @0x8381D0C8 — sift-up (bubble toward the root) one entry of an obstacle_path's binary min-heap
 * (ordered by step total_distance), used to maintain the pathfinding open list. */

#include <stdint.h>
#include "headers/obstacle_path.h"

void heap_up(obstacle_path *path, int16_t heap_index)
{
    __int16 child_index = heap_index;

    if ( heap_index > 0 )
    {
        do
        {
            __int16 parent_index = (__int16)(child_index - 1) >> 1;
            __int16 child_step_index = path->heap[child_index];
            __int16 parent_step_index = path->heap[parent_index];

            if ( path->steps[parent_step_index].total_distance <= path->steps[child_step_index].total_distance )
                break;

            path->heap[parent_index] = child_step_index;
            path->heap[child_index] = parent_step_index;
            child_index = parent_index;
        }
        while ( child_index > 0 );
    }
}
