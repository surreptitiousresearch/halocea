/* device_set_actual_position @0x837B5B20 — immediately set the actual value of a device's position group
 * (object data +0x204 = position device group index). No-op for an invalid device or unassigned group. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/blam_data_globals.h"

extern void device_group_set_actual_value(int16_t group_index, float value);

void device_set_actual_position(int device_index, float value)
{
    if ( device_index == -1 )
        return;
    device_datum *device = (device_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    int position_group = (uint16_t)device->device.position_group_index;
    if ( position_group != 0xFFFF )
        device_group_set_actual_value(position_group, value);
}
