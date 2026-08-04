/* rasterizer_transparent_geometry_set_group_pending_status 0x83768540 — flip a group's "pending" bit in the
 * 384-bit bucket vector. The group must live inside the transparent_geometry_groups[] pool; its array index
 * selects the dword (index>>5) and bit (index&0x1F).
 *
 * DEVIATION: the status argument is inverted relative to its name — status==0 SETS the pending bit, a non-zero
 * status CLEARS it (matching the disassembly: the OR is the default, the AND-NOT is taken when status != 0). */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"

extern unsigned char transparent_geometry_group_buckets[48]; /* transparent_geometry_group_pending_bits — 384-bit pending bit vector */

void rasterizer_transparent_geometry_set_group_pending_status(const transparent_geometry_group *group,
                                                              uint8_t status)
{
    int16_t group_index = -1;

    if ( group >= transparent_geometry_groups
      && group < &transparent_geometry_groups[transparent_geometry_group_count] )
        group_index = group - transparent_geometry_groups;

    if ( group_index != -1 )
    {
        int *buckets = (int *)transparent_geometry_group_buckets;
        /* decompiler hoisted the bit mask into a temp; macros re-inline it (same value) */
        if ( status )
            BIT_VECTOR_CLEAR_FLAG(buckets, group_index);
        else
            BIT_VECTOR_SET_FLAG(buckets, group_index);
    }
}
