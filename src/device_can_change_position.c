/* device_can_change_position @0x837B5198 — decides whether a device (door/lift/etc.) is currently
 * allowed to move toward its desired position. Requires: the device's "position can change" override
 * (flag 0x2 of the device-state word) is clear; its device group either permits change or is not in a
 * "one-shot already triggered" state; and its power group is fully powered (value == 1.0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/device_group_datum.h"
#include "headers/device_group_flags.h"
#include "headers/device_datum_flags.h"
#include "headers/blam_data_globals.h"


uint8_t device_can_change_position(int device_index)
{
    int result = 0;
    device_datum *device = (device_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;

    uint16_t device_group_index = (uint16_t)device->device.position_group_index;
    if (device_group_index != 0xFFFF)
    {
        int change_allowed = 1;
        device_group_datum *position_group =
            DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, device_group_index);
        device_group_datum *power_group = DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum,
                                                             device->device.power_group_index);
        int16_t group_flags = position_group->flags;
        if ((group_flags & (1 << _device_group_can_change_only_once_bit)) != 0)
            change_allowed = (group_flags & (1 << _device_group_changed_once_bit)) == 0;
        result = (device->device.flags & (1u << _device_not_usable_bit)) == 0 && change_allowed;
        if (power_group->desired_value != 1.0f)
            return 0;
    }
    return result;
}
