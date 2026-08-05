/* device_set_power @0x837B5D08 — set a device object's power level (device.power), flag it changed
 * (device.flags bit 0x4), and push the value to its associated power device group. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/device_datum_flags.h"
#include "headers/blam_data_globals.h"

extern uint8_t device_group_set_desired_value(int16_t group_index, float desired_value);

void device_set_power(int device_index, float power)
{
    if ( device_index == -1 )
        return;
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    unsigned int flags = device->device.flags;
    device->device.power = power;
    int16_t power_group = device->device.power_group_index;
    device->device.flags = flags | (1u << _device_animation_changed_bit);
    device_group_set_desired_value(power_group, power);
}
