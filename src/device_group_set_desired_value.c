/* device_group_set_desired_value @0x837B5938 — request a new value (clamped to [0,1]) for a device group.
 * If the value actually changes (and the group is not locked mid-change), it is stored, the group is flagged
 * changing, and every device bound to the group fires its activation/deactivation effect or sound: each
 * member looks up an effect/sound tag reference in its definition (deactivate ref +0x1EC when the new value
 * is 0, activate ref +0x1FC otherwise) and spawns it. Returns nonzero if the value changed.
 *
 * DEVIATION: effect_new_from_object's trailing color/impulse-field arguments are loaded as 0 at the call
 * site (verified in disassembly); the decompiler rendered them as uninitialized locals. Passed as null. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/device_datum.h"
#include "headers/device_group_datum.h"
#include "headers/device_group_flags.h"
#include "headers/device_definition.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_iterator.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

struct effect_vector_field;

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern uint32_t tag_get_group_tag(int16_t tag_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

uint8_t device_group_set_desired_value(int16_t group_index, float desired_value)
{
    float clamped = desired_value;
    int changed = 0;
    if ( desired_value < 0.0f )
        clamped = 0.0f;
    else if ( desired_value > 1.0f )
        clamped = 1.0f;

    int16_t group = group_index;
    if ( group_index != -1 )
    {
        device_group_datum *group_datum =
            DATA_ARRAY_ELEMENT(device_groups_data, device_group_datum, group_index);
        if ( group_datum->desired_value != clamped )
        {
            __int16 flags = group_datum->flags;
            /* a "change only once" group that has already changed once is locked */
            if ( (flags & (1u << _device_group_can_change_only_once_bit)) == 0
              || (flags & (1u << _device_group_changed_once_bit)) == 0 )
            {
                group_datum->desired_value = clamped;
                changed = 1;
                group_datum->flags = flags | (1u << _device_group_changed_once_bit);
            }
        }
    }

    if ( changed )
    {
        object_iterator iterator;
        object_iterator_new(&iterator, object_mask_device, 0);
        for ( __int16 *object = object_iterator_next(&iterator);
              object;
              object = object_iterator_next(&iterator) )
        {
            device_datum *device = (device_datum *)object;
            device_definition *definition_data =
                TAG_GET(device_definition, device->definition_index);
            if ( device->device.power_group_index == group )
            {
                int effect_index = clamped <= 0.0f ? definition_data->device.depowered_effect.index
                                                   : definition_data->device.repowered_effect.index;
                int index = iterator.index;
                if ( effect_index != -1 )
                {
                    device_datum *effect_device = (device_datum *)DATA_ARRAY_ELEMENT(
                        object_header_data, object_header_datum, iterator.index)->datum;
                    unsigned int group_tag = tag_get_group_tag(effect_index);
                    if ( group_tag == 0x65666665u /* 'effe' */ )
                    {
                        effect_new_from_object(effect_index, index, index, -1,
                                               effect_device->device.position,
                                               effect_device->device.power,
                                               nullptr, nullptr);
                    }
                    else if ( group_tag == 0x736E6421u /* 'snd!' */ )
                    {
                        object_impulse_sound_new(index, effect_index, -1, global_origin3d, global_forward3d, 1.0f, 0);
                    }
                }
            }
        }
    }

    return changed;
}
