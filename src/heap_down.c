/* heap_down @0x8381D150 — standard min-heap sift-down of `path->heap[heap_index]` by
 * `path->steps[...].total_distance`, the counterpart to path_add_step's sift-up.
 *
 * DEVIATION — same offset-folding artifact as the rest of this arc: the swap is expressed as
 * `*(_WORD *)((char *)&path->radius + 2*(index + 2585))`, which is just `&path->heap[index]` (heap sits at
 * obstacle_path offset 0x1432 = 5170 bytes = 2*2585; `&path->radius` is offset 0 into that same base), and is
 * reconstructed below as direct `path->heap[index]` indexing. */

#include <stdint.h>
#include "headers/obstacle_path.h"

void heap_down(obstacle_path *path, int16_t heap_index)
{
    while (heap_index < path->heap_count)
    {
        __int16 parent_index = heap_index;
        int heap_count = path->heap_count;

        __int16 left_index = 2 * heap_index + 1;
        __int16 right_index = 2 * heap_index + 2;

        if (left_index < heap_count
                && path->steps[path->heap[heap_index]].total_distance > path->steps[path->heap[left_index]].total_distance)
            heap_index = left_index;

        if (right_index < heap_count
                && path->steps[path->heap[heap_index]].total_distance > path->steps[path->heap[right_index]].total_distance)
            heap_index = right_index;

        if (heap_index == parent_index)
            break;

        __int16 swap_temp = path->heap[heap_index];
        path->heap[heap_index] = path->heap[parent_index];
        path->heap[parent_index] = swap_temp;
    }
}
