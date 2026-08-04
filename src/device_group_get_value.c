/* device_group_get_value @0x837B5120 — return a device group's current value (device group datum +0x4). */

#include "headers/data_array.h"
#include "headers/device_group_datum.h"
#include "headers/blam_data_globals.h"


float device_group_get_value(int group_index)
{
    return DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, group_index)->desired_value;
}
