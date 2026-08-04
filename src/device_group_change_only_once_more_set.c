/* device_group_change_only_once_more_set @0x837B50C8 — set the device group's "change only once more" flag
 * (_device_group_can_change_only_once_bit) when requested, then always clear the
 * _device_group_changed_once_bit. No-op for an invalid group.
 *
 * DEVIATION: the binary writes the flags word twice (set can_change_only_once, then clear changed_once); reproduced as
 * written. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/device_group_flags.h"
#include "headers/device_group_datum.h"
#include "headers/blam_data_globals.h"


void device_group_change_only_once_more_set(int device_group_index, uint8_t change_only_once)
{
    if ( device_group_index == -1 )
        return;
    device_group_datum *group = DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, device_group_index);
    __int16 flags = change_only_once ? (group->flags | (1u << _device_group_can_change_only_once_bit)) : (group->flags & ~(1u << _device_group_can_change_only_once_bit));
    group->flags = flags;
    group->flags = flags & ~(1u << _device_group_changed_once_bit);
}
