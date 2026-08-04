/* rasterizer_transparent_geometry_new_group2 @0x837683AC — sibling pool to rasterizer_transparent_geometry_new_group
 * (see that file): allocates the next transparent-geometry group from a second, smaller static pool
 * (capacity 32) used for the camouflage/skinned immediate-draw path. Returns null when full. */

#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


transparent_geometry_group *rasterizer_transparent_geometry_new_group2(void)
{
    transparent_geometry_group *result = nullptr;
    int index = transparent_geometry_group_count2;
    if ( transparent_geometry_group_count2 < 32 )
    {
        result = &transparent_geometry_groups2[transparent_geometry_group_count2];
        result->sorted_index = transparent_geometry_group_count2;
        transparent_geometry_group_count2 = index + 1;
    }
    return result;
}
