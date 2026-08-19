/* device_new @0x837B4A58 — initialize a freshly placed device object: clear its position/power group
 * indices (device.position_group_index / device.power_group_index) and set the object.flags
 * shadowless bit (_object_shadowless_bit, 0x12 => 0x40000). */
#include <stdint.h>

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


uint8_t device_new(int device_index)
{
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;

    device->device.position_group_index = -1;
    device->device.power_group_index = -1;
    device->object.flags |= (1u << _object_shadowless_bit);
    return 1;
}
