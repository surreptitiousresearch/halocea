/* rasterizer_transparent_geometry_get_group_from_presorted_index @0x83768468 */
#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


const transparent_geometry_group *rasterizer_transparent_geometry_get_group_from_presorted_index(
        int16_t group_presorted_index)
{
    return &transparent_geometry_groups[group_presorted_index];
}
