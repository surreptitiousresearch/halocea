#include "headers/blam_data_globals.h"
/* rasterizer_transparent_geometry_begin @0x83768344 — resets the per-frame transparent-geometry
 * sort lists: clears the group counters and the 0x30-byte group-bucket table. */

extern unsigned char transparent_geometry_group_buckets[48]; /* transparent_geometry_group_pending_bits */

extern void *memset(void *dst, int value, unsigned int size);

void rasterizer_transparent_geometry_begin(void)
{
    transparent_geometry_group_count = 0;
    transparent_geometry_attached_group_count = 0;
    memset(transparent_geometry_group_buckets, 0, sizeof(transparent_geometry_group_buckets));
    transparent_geometry_group_count2 = 0;
}
