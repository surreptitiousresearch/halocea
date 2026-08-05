/* device_group_delete @0x837B5408 — deletes a device group only if bit 2 of its DB-named
 * device_group_datum.flags word is set; a -1 index (already-deleted/none) is a no-op. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/device_group_datum.h"
#include "headers/device_group_flags.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void device_group_delete(int device_group_index)
{
    int16_t index = (int16_t)device_group_index;
    if ( index == -1 )
        return;
    /* (8*index)&0x7FFF8 is the plain 8-byte stride for any 16-bit index */
    device_group_datum *group = DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, index);
    if ( group->flags & (1u << _device_group_runtime_bit) )
        datum_delete(device_groups_data, index);
}
