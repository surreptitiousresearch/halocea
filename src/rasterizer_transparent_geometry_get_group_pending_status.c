/* rasterizer_transparent_geometry_get_group_pending_status @0x837684C8 — query a group's "pending" bit in
 * the 384-bit bucket vector (same layout as rasterizer_transparent_geometry_set_group_pending_status.c).
 * Groups outside the transparent_geometry_groups[] pool report pending (true). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"

extern unsigned char transparent_geometry_group_buckets[48]; /* transparent_geometry_group_pending_bits — 384-bit pending bit vector */

uint8_t rasterizer_transparent_geometry_get_group_pending_status(const transparent_geometry_group *group)
{
    int16_t group_index = -1;

    if ( group >= transparent_geometry_groups
      && group < &transparent_geometry_groups[transparent_geometry_group_count] )
        group_index = group - transparent_geometry_groups;

    if ( group_index != -1 )
    {
        int *buckets = (int *)transparent_geometry_group_buckets;

        return !BIT_VECTOR_TEST_FLAG(buckets, group_index);
    }
    return 1;
}
