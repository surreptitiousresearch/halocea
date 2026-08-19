/* path_get_step_index @0x8381D030 */
#include <stdint.h>
#include "headers/obstacle_path.h"

uint16_t path_get_step_index(obstacle_path *path, int16_t heap_index)
{
    return (unsigned short)path->heap[heap_index];
}
