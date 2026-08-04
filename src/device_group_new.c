/* device_group_new @0x837B5398 — DEVIATION: the decompiler declared a `flags` parameter (never read) and
 * fabricated a bogus 3rd `a3` parameter for the value actually stored. disasm_range(0x837B5398,0x837B53E8)
 * confirms only 2 arguments are used: f1 (desired_value) and r4, stored as a 16-bit value at offset+2 of
 * the new device_group entry (desired_value itself lands at offset+4). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/device_group_datum.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);

int16_t device_group_new(float desired_value, int16_t flags)
{
    __int16 index = (__int16)datum_new(device_groups_data);

    if ( index != -1 )
    {
        device_group_datum *entry = DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, index);

        entry->flags = flags;
        entry->desired_value = desired_value;
    }
    return index;
}
