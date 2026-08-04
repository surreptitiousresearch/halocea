/* device_effect_new @0x837B52E8 — spawns a device's feedback effect when it actuates. If the referenced
 * tag is an effect ('effe') it is created on the device (scaled by the device's power/position fields); if
 * it is a sound ('snd!') an impulse sound is played at the device. No-op for an unset effect index.
 *
 * DEVIATION: the decompiler left the effect color/impulse-field args uninitialized and rendered the
 * sound's is_local_player as 0x82000000 — the disassembly loads 0/0/0 for all three. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern uint32_t tag_get_group_tag(int16_t tag_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

void device_effect_new(int device_index, int effect_index)
{
    if (effect_index == -1)
        return;

    device_datum *device = (device_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;

    unsigned int group_tag = tag_get_group_tag(effect_index);
    if (group_tag == 0x65666665u /* 'effe' */)
    {
        effect_new_from_object(effect_index, device_index, device_index, -1,
                               device->device.position, device->device.power, nullptr, nullptr);
    }
    else if (group_tag == 0x736E6421u /* 'snd!' */)
    {
        object_impulse_sound_new(device_index, effect_index, -1, global_origin3d, global_forward3d,
                                 1.0f, 0);
    }
}
