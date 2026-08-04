/* rasterizer_transparent_geometry_next_group @0x83768408 — walks to the next group in sorted draw order
 * (by group->sorted_index + 1); a null group input returns null (no wraparound / no starting group). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


const transparent_geometry_group *rasterizer_transparent_geometry_next_group(const transparent_geometry_group *group)
{
    if (!group)
        return group;

    int16_t next_sorted_index = (int16_t)(group->sorted_index + 1);
    if (next_sorted_index >= transparent_geometry_group_count)
        return nullptr;

    return &transparent_geometry_groups[transparent_geometry_group_sorted_indices[next_sorted_index]];
}
