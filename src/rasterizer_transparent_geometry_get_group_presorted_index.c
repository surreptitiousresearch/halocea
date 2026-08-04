/* rasterizer_transparent_geometry_get_group_presorted_index @0x83768498 — returns a group's index within
 * the main transparent_geometry_groups pool (see rasterizer_transparent_geometry_new_group.c), or -1 if the
 * group doesn't belong to that pool (e.g. it's the group2 pool, or the decal/camouflage singleton). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


int16_t rasterizer_transparent_geometry_get_group_presorted_index(const transparent_geometry_group *group)
{
    if (group >= transparent_geometry_groups && group < &transparent_geometry_groups[transparent_geometry_group_count])
        return (int16_t)(group - transparent_geometry_groups);
    return -1;
}
