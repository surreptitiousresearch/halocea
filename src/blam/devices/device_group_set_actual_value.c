/* device_group_set_actual_value @0x837B4F18 — immediately set a device group's value (clamped to [0,1]) and
 * propagate it to every device object bound to that group, updating the object's actual value, zeroing its
 * change rate, and flagging it changed. Power-group members (object +0x1F8) take the value in their
 * power slot (+0x1FC); position-group members (object +0x204) take it in their position slot (+0x208). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/device_datum.h"
#include "headers/device_datum_flags.h"
#include "headers/device_group_datum.h"
#include "headers/object_iterator.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


void device_group_set_actual_value(int16_t group_index, float value)
{
    float clamped = value;
    if ( value < 0.0f )
        clamped = 0.0f;
    else if ( value > 1.0f )
        clamped = 1.0f;

    DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, group_index)->desired_value = clamped;

    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_device, 0);
    for ( int16_t *object = object_iterator_next(&iterator);
          object;
          object = object_iterator_next(&iterator) )
    {
        device_datum *device = (device_datum *)object;
        if ( device->device.power_group_index == group_index )
        {
            int flags = device->device.flags;
            device->device.power = clamped;
            device->device.power_velocity = 0.0f;
            device->device.flags = flags | (1u << _device_animation_changed_bit);
        }
        if ( device->device.position_group_index == group_index )
        {
            int flags = device->device.flags;
            device->device.position = clamped;
            device->device.position_velocity = 0.0f;
            device->device.flags = flags | (1u << _device_animation_changed_bit);
        }
    }
}
