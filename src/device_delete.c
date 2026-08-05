/* device_delete @0x837B55C4 — drop a device's membership in up to two device groups
 * (device.power_group_index and device.position_group_index) — each is deleted from
 * device_groups_data only if its flags word (bit 0x4) is set. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/device_group_datum.h"
#include "headers/device_group_flags.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void device_delete(int device_index)
{
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    device_group_datum *groups = (device_group_datum *)device_groups_data->data;

    int16_t group1 = device->device.power_group_index;
    if ( group1 != -1 && (groups[(uint16_t)group1].flags & (1u << _device_group_runtime_bit)) != 0 )
        datum_delete(device_groups_data, group1);

    int16_t group2 = device->device.position_group_index;
    if ( group2 != -1 && (groups[(uint16_t)group2].flags & (1u << _device_group_runtime_bit)) != 0 )
        datum_delete(device_groups_data, group2);
}
