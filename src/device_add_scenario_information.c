/* device_add_scenario_information @0x837B5B60 — wire a placed device object up to its scenario-assigned
 * power/position device groups, creating a fresh group (seeded to fully-on or fully-off per the
 * scenario placement's flags) for either that's not already assigned (-1). Caches each group's current
 * value onto the device object, and ORs in two device-specific flag bits from the scenario placement.
 *
 * The power-group value lookup's `__ROL4__(power_group_index, 3)` is equivalent to the position-group
 * lookup's plain `8 * index` masked form (device group indices are always small; the rotate's high-bit
 * wraparound never triggers), simplified here to match. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/device_datum_flags.h"
#include "headers/device_group_flags.h"
#include "headers/device_group_datum.h"
#include "headers/scenario_device_datum.h"
#include "headers/scenario_device_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t device_group_new(float desired_value, int16_t flags);

void device_add_scenario_information(unsigned int device_index, scenario_device_datum *scenario_device)
{
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    device_group_datum *groups = (device_group_datum *)device_groups_data->data;

    int16_t power_group_index = scenario_device->power_group_index;
    if ( power_group_index == -1 )
    {
        float desired_value = (scenario_device->flags & (1u << _scenario_device_initially_off_bit)) ? 0.0f : 1.0f;
        power_group_index = device_group_new(desired_value, 1 << _device_group_runtime_bit); /* DEVIATION: li r4,4 @0x837B5BE0 -- flags constant, not device_index */
    }
    device->device.power_group_index = power_group_index;

    int16_t position_group_index = scenario_device->position_group_index;
    if ( position_group_index == -1 )
    {
        float desired_value = (scenario_device->flags & (1u << _scenario_device_initially_open_bit)) ? 1.0f : 0.0f;
        position_group_index = device_group_new(desired_value,
                (1 << _device_group_runtime_bit) |
                ((scenario_device->flags >> _scenario_device_changes_only_once_bit) & 1)); /* DEVIATION: li r4,4; rlwimi r4,r11,30,31,31 @0x837B5C1C-20 -- propagates changes-only-once */
    }
    device->device.position_group_index = position_group_index;

    device->device.power = groups[(uint16_t)power_group_index].desired_value;
    device->device.position = groups[(uint16_t)position_group_index].desired_value;

    if ( scenario_device->flags & (1u << _scenario_device_position_reversed_bit) )
        device->device.flags |= (1u << _device_position_reversed_bit);
    if ( scenario_device->flags & (1u << _scenario_device_not_usable_bit) )
        device->device.flags |= (1u << _device_not_usable_bit);
}
