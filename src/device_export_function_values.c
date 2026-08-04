/* device_export_function_values @0x837B4AA0 — compute and store the four exported function values of a device
 * (door/machine/control) object for this tick, so shaders/animations driven by "device functions" can sample
 * them. For each of the 4 function slots the device tag names a source (device.function_modes[slot]); the value
 * is derived from the device object's live state: power (1), scaled power velocity (2), position (3), scaled
 * position velocity (4), or a machine on/off/one-shot power test (5, using the device group state). Slot source
 * 6 maps the raw delay-tick value against runtime_delay_ticks; sources >= 7 export 0. Results are written to
 * object.incoming_function_values[slot].
 *
 * DEVIATION: the decompiler modeled machine.flags as the high dword of a phantom __int64 `v1` (initialized from
 * a bogus &XBM__CaptureCompletionSignalMarker that has no instruction backing it — disasm 0x837B4AB0 has no
 * such store), so its `v1 & 0x1/2/4_00000000LL` tests are just `flags & 1/2/4` on machine.flags. */

#include "headers/device_function_mode.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/machine_datum.h"
#include "headers/machine_definition.h"
#include "headers/device_group_datum.h"
#include "headers/device_group_flags.h"
#include "headers/machine_datum_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);

void device_export_function_values(int device_index)
{
    int slot = 0;
    machine_datum *device = (machine_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    machine_definition *definition = TAG_GET(machine_definition, device->definition_index);

    do
    {
        __int16 function_type = definition->device.function_modes[slot];
        if ( !function_type )
            goto next_slot;

        float value = 0.0f;
        if ( function_type >= _device_function_power && function_type <= _device_function_delay )
        {
            switch ( function_type )
            {
                case _device_function_power:
                    value = device->device.power;
                    break;
                case _device_function_change_in_power:
                {
                    float power_velocity = device->device.power_velocity;
                    if ( power_velocity != 0.0f )
                        value = (float)(__fabs(power_velocity) / definition->device.runtime_maximum_power_velocity);
                    break;
                }
                case _device_function_position:
                    value = device->device.position;
                    break;
                case _device_function_change_in_position:
                {
                    float position_velocity = device->device.position_velocity;
                    if ( position_velocity != 0.0f )
                        value = (float)(__fabs(position_velocity) / definition->device.runtime_maximum_powered_position_velocity);
                    break;
                }
                case _device_function_locked:
                {
                    if ( device->device.power == 0.0f )
                        value = 1.0f;
                    if ( (unsigned __int16)device->object.type != object_type_machine || (unsigned __int16)device->device.position_group_index == 0xFFFF )
                        break;
                    unsigned int flags = device->machine.flags;
                    device_group_datum *group = DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, device->device.position_group_index);
                    if ( (flags & (1u << _machine_does_not_operate_automatically_bit)) != 0 || (flags & (1u << _machine_one_sided_bit)) != 0 )
                        value = 1.0f;
                    unsigned __int16 group_flags = group->flags;
                    if ( (group_flags & (1u << _device_group_can_change_only_once_bit)) != 0 && (group_flags & (1u << _device_group_changed_once_bit)) != 0 )
                        value = 1.0f;
                    if ( device->device.position != 1.0f && (flags & (1u << _machine_never_appears_locked_bit)) == 0 )
                        break;
                    value = 0.0f;
                    break;
                }
                default: /* _device_function_delay */
                {
                    float scale = definition->device.runtime_delay_ticks;
                    if ( scale > 0.0f )
                    {
                        float raw = (float)(int)device->device.delay_ticks;
                        if ( raw != scale )
                            value = (raw / scale);
                    }
                    break;
                }
            }
        }

        device->object.incoming_function_values[slot] = value;
next_slot:
        slot = (__int16)(slot + 1);
    }
    while ( slot < 4 );
}
