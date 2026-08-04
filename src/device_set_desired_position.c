/* device_set_desired_position @0x837B5CC0 — request a new value for a device's position group
 * (device.position_group_index). Returns nonzero if the desired value actually changed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t device_group_set_desired_value(int16_t group_index, float desired_value);

uint8_t device_set_desired_position(int device_index, float value)
{
    if ( device_index == -1 )
        return 0;
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    unsigned __int16 position_group = device->device.position_group_index;
    if ( position_group == 0xFFFF )
        return 0;
    return device_group_set_desired_value(position_group, value);
}
