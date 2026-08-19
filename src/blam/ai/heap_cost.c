/* heap_cost @0x8381D090 — total_distance of the pathfinding step referenced by a heap slot. */

#include <stdint.h>
#include "headers/obstacle_path.h"

float heap_cost(obstacle_path *path, int16_t heap_index)
{
    return path->steps[path->heap[heap_index]].total_distance;
}
