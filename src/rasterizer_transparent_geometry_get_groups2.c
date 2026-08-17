/* rasterizer_transparent_geometry_get_groups2 @0x837683E0 */
#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


const transparent_geometry_group *rasterizer_transparent_geometry_get_groups2(int16_t *count)
{
    const transparent_geometry_group *result = transparent_geometry_groups2;
    if (count)
        *count = transparent_geometry_group_count2;
    return result;
}
