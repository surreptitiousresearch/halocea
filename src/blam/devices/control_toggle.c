/* control_toggle @0x837E9B30 — actuates a control (device) object: computes the desired device-group
 * value from the control definition's toggle type, drives it, and spawns the matching device effect
 * (on / off / failure). No-op if the control has no device group.
 *
 * control.type selects the value: 0 = flip from the current group value (a true toggle), 1 = force on
 * (1.0), 2 = force off (0.0), 3 = the definition's call_value. Type > 3 is invalid; CAVEAT: as-shipped —
 * the binary loads desired_value from an uninitialized stack float there (lfs f31, 0x90+var_40(r1)
 * @0x837E9BF8; no store to that slot in the function), reproduced as an indeterminate value. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/control_definition.h"
#include "headers/control_type.h"
#include "headers/device_group_datum.h"
#include "headers/blam_data_globals.h"


extern void hcex_control_toggle(int id, float val);
extern uint8_t device_group_set_desired_value(int16_t group_index, float desired_value);
extern void device_effect_new(int device_index, int effect_index);

void control_toggle(int control_index)
{
    device_datum *object = (device_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, control_index)->datum;
    control_definition *definition = TAG_GET(control_definition, object->definition_index);

    uint16_t device_group_index = (uint16_t)object->device.position_group_index;
    if (device_group_index == 0xFFFF)
        return;

    unsigned int control_type = (uint16_t)definition->control.type;

    float desired_value;   /* type > 3 leaves this indeterminate, matching the binary */
    if (control_type >= NUMBER_OF_CONTROL_TYPES)
    {
        /* invalid control type: value left uninitialized */
    }
    else if (control_type == _control_on_button)
    {
        desired_value = 1.0f;
    }
    else if (control_type == _control_off_button)
    {
        desired_value = 0.0f;
    }
    else if (control_type == _control_call_button)
    {
        desired_value = definition->control.call_value;
    }
    else  /* _control_toggle_switch — flip from the current device-group value */
    {
        float current = DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum,
                                           device_group_index)->desired_value;
        desired_value = current <= 0.5f ? 1.0f : 0.0f;
    }

    hcex_control_toggle(control_index, desired_value);

    int effect_index;
    if (device_group_set_desired_value(object->device.position_group_index, desired_value))
        effect_index = desired_value > 0.5f ? definition->control.on_effect.index
                                            : definition->control.off_effect.index;
    else
        effect_index = definition->control.deny_effect.index;

    device_effect_new(control_index, effect_index);
}
