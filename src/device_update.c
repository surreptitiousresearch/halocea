/* device_update @0x837B5658 — advance one scenario device's (door/lift/machine) power and position toward the
 * targets published by its device groups, spawning the configured device effects on transitions. It looks up the
 * device object and its device tag definition, then:
 *   Power: if the device has a power group, accelerate its power value/velocity toward the group's power using the
 *   definition's power acceleration/velocity limits; mark the object dirty if the power changed.
 *   Position: if the device has a position group, accelerate its position toward the group's position. The
 *   acceleration and velocity limits are interpolated between the definition's open/closed limits by the current
 *   power. A per-side "fully closed" dwell counter (+528) spawns the close effect once when the position has
 *   settled at 0; otherwise the position velocity is clamped to the interpolated limit, and reaching / failing to
 *   reach the target spawns the open/close (or the two closed) effects. Marks the object dirty on a position
 *   change. Returns 1 if either accelerate step reported completion (a state change occurred).
 *
 * DEVIATION: accelerate_to_position's final circular_position arg is 0 (disasm `li r10,0`; the decompiler's
 * 0x82000000 is a stale FPR-shadow GPR). The (float)__int64 counter comparison is (float)(int)dwell_counter.
 *
 * BUGFIX vs. prior source: the device tag definition is element[index].base_address (loaded at
 * `lwz r30, 0x14(r7)`, disasm 0x837B56D8), i.e. `*(int *)TAG_INSTANCE(index)` — the same read device_export
 * uses. The earlier hand-source did `*(int *)(TAG_INSTANCE(index) + 20)`, which double-counts the +0x14
 * base_address offset the TAG_INSTANCE macro already applies and reads into the next table entry. Corrected. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/device_definition.h"
#include "headers/device_group_datum.h"
#include "headers/global_tag_instances.h"
#include <math.h>
#include "headers/device_datum_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t accelerate_to_position(float *position_reference, float *velocity_reference, float position_desired, float acceleration_maximum, float velocity_maximum, float position_lower_bound, float position_upper_bound, uint8_t circular_position);
extern void device_effect_new(int device_index, int effect_index);

uint8_t device_update(int device_index)
{
    int changed = 0;
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    int16_t power_group = device->device.power_group_index;
    device_definition *definition = TAG_GET(device_definition, device->definition_index);
    device_group_datum *groups = (device_group_datum *)device_groups_data->data;

    if ( (uint16_t)power_group != 0xFFFF )
    {
        float power = device->device.power;
        float power_target = groups[(uint16_t)power_group].desired_value;
        if ( power_target != power || device->device.power_velocity != 0.0f )
        {
            if ( !accelerate_to_position(&device->device.power, &device->device.power_velocity, power_target,
                    definition->device.runtime_maximum_power_acceleration,
                    definition->device.runtime_maximum_power_velocity, 0.0f, 1.0f, 0) )
                changed = 1;
            if ( power != device->device.power )
                device->device.flags |= (1u << _device_animation_changed_bit);
        }
    }

    int16_t position_group = device->device.position_group_index;
    if ( (uint16_t)position_group == 0xFFFF )
        return changed;

    float position_target = device->device.position;
    device_group_datum *group = &groups[(uint16_t)position_group];
    float group_position = group->desired_value;
    if ( group_position == position_target && device->device.position_velocity == 0.0f )
    {
        device->device.delay_ticks = 0;
        return changed;
    }

    float position_velocity = device->device.position_velocity;
    float power_now = device->device.power;
    float acceleration_limit = ((definition->device.runtime_maximum_depowered_position_acceleration * (1.0f - power_now))
            + (definition->device.runtime_maximum_powered_position_acceleration * power_now));
    float velocity_limit = ((definition->device.runtime_maximum_depowered_position_velocity * (1.0f - power_now))
            + (definition->device.runtime_maximum_powered_position_velocity * power_now));
    int16_t dwell_counter = device->device.delay_ticks;
    char velocity_positive = position_velocity > 0.0f;

    if ( (float)dwell_counter < definition->device.runtime_delay_ticks
      && position_target == 0.0f && group_position >= position_target )
    {
        device->device.delay_ticks = dwell_counter + 1;
        if ( (int16_t)(dwell_counter + 1) == 1 )
            device_effect_new(device_index, definition->device.delay_effect.index);
        return changed;
    }

    float position_velocity_pre = device->device.position_velocity;
    if ( __fabs(position_velocity) > velocity_limit )
    {
        float clamped_velocity;
        if ( position_velocity <= 0.0f )
            clamped_velocity = -velocity_limit;
        else
            clamped_velocity = ((definition->device.runtime_maximum_depowered_position_velocity * (1.0f - power_now))
                    + (definition->device.runtime_maximum_powered_position_velocity * power_now));
        device->device.position_velocity = clamped_velocity;
    }

    if ( accelerate_to_position(&device->device.position, &device->device.position_velocity, group->desired_value,
            acceleration_limit, velocity_limit, 0.0f, 1.0f, 0) )
    {
        if ( velocity_positive )
            device_effect_new(device_index, definition->device.positive_stop_effect.index);
        else
            device_effect_new(device_index, definition->device.negative_stop_effect.index);
    }
    else
    {
        float velocity_now = device->device.position_velocity;
        if ( velocity_now != 0.0f && (velocity_now * position_velocity_pre) <= 0.0f )
        {
            int effect;
            if ( velocity_now <= position_velocity_pre )
                effect = definition->device.negative_start_effect.index;
            else
                effect = definition->device.positive_start_effect.index;
            device_effect_new(device_index, effect);
        }
        changed = 1;
    }

    if ( position_target != device->device.position )
        device->device.flags |= (1u << _device_animation_changed_bit);
    return changed;
}
