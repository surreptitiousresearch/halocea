/* device_can_change_position @0x837B5188 — decides whether a device (door/lift/etc.) is currently
 * allowed to move toward its desired position. Requires: the device's "position can change" override
 * (flag 0x2 of the device-state word) is clear; its device group either permits change or is not in a
 * "one-shot already triggered" state; and its power group is fully powered (value == 1.0).
 *
 * CAVEAT — shipped bug, faithfully reconstructed. Only the POSITION group index is guarded against
 * 0xFFFF (lhz r10, 0x204 / cmplwi 0xFFFF / beqlr @0x837B51B0-B8). The POWER group index is loaded
 * zero-extended and scaled straight into an address with no sentinel test at all
 * (lhz r5, 0x1F8 @0x837B51C0, rotlwi r8, r5, 3 @0x837B51C8), so a device with a position group but
 * no power group reads device_group_datum element 65535 — far past the array. The DATA_ARRAY_ELEMENT
 * (uint16_t) truncation reproduces that zero-extension exactly; do not add the missing guard. */

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
