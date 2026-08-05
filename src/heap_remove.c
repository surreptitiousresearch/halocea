/* heap_remove @0x8381D348 — pop the root of the pathfinding heap: return it, move the last element to the
 * root, and sift it back down. -1 if the heap is empty. */

#include <stdint.h>
#include "headers/obstacle_path.h"

extern void heap_down(obstacle_path *path, int16_t heap_index);

int16_t heap_remove(obstacle_path *path)
{
    if ( path->heap_count <= 0 )
        return -1;

    int16_t root = path->heap[0];
    int16_t new_count = path->heap_count - 1;

    path->heap_count = new_count;
    path->heap[0] = path->heap[new_count];
    heap_down(path, 0);
    return root;
}
