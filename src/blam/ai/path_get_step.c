/* path_get_step @0x8381D010 */
#include <stdint.h>
#include "headers/obstacle_path.h"

step * path_get_step(obstacle_path *path, int16_t step_index)
{
    return &path->steps[step_index];
}
