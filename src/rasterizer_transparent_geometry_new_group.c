/* rasterizer_transparent_geometry_new_group 0x83768370 — allocate the next transparent-geometry group from the
 * static pool (capacity 384), stamping it with its own index as the initial sort key. Returns null when full. */

#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


transparent_geometry_group *rasterizer_transparent_geometry_new_group(void)
{
    transparent_geometry_group *result = nullptr;
    int index = transparent_geometry_group_count;
    if ( transparent_geometry_group_count < 384 )
    {
        result = &transparent_geometry_groups[transparent_geometry_group_count];
        result->sorted_index = transparent_geometry_group_count;
        transparent_geometry_group_count = index + 1;
    }
    return result;
}
