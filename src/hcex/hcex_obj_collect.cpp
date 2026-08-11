/* hcex_obj_collect @0x83681FD0 — one-time bridge pass that mirrors the Blam world into the hcex render
 * layer after a map loads. For every object it initializes and customizes the hcex proxy; for every light
 * attached to a valid object at a "flashlight" marker it creates an hcex light; and for every effect it
 * seeds the hcex location-based effect proxy.
 *
 * DEVIATION (#119): `void`, not `int` — the earlier `return k;` published loop residue as a result.
 * r3 at the epilogue is the value of the loop's own `bl data_next_index` @0x8368212C, which the exit
 * test `cmpwi cr6, r3, -1` / `bne cr6` @0x83682134-0x83682138 has just proven to be -1, and the sole
 * caller (game_state_call_after_load_procs, `bl` @0x83684594) ignores r3 — its extern already spelled
 * `void`, so the definition was the half that diverged. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/effect_datum.h"
#include "../headers/object_datum.h"
#include "../headers/light_datum.h"
#include "../headers/object_type.h"

#include "headers/data_array.h"
extern "C" data_array *object_header_data;
extern "C" data_array *light_data;
extern "C" data_array *effect_data;

extern "C" int data_next_index(const data_array *data, int16_t index);
extern "C" void  hcex_obj_init(int id);
extern "C" void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern "C" void  hcex_obj_customize(int id, int tag_index);
extern "C" const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index);
extern "C" char *tag_get_name(int tag_index);
extern "C" void  hcex_create_light(int light_id, int object_index, const char *name);
extern "C" effect_datum *effect_try_and_get(int index);
extern "C" void  hcex_init_effect_by_locations(int effect_index, int object_index, float scale);
extern "C" int   strcmp(const char *a, const char *b);

extern "C" void hcex_obj_collect(void)
{
    for (int i = data_next_index(object_header_data, -1); i != -1; i = data_next_index(object_header_data, i))
    {
        hcex_obj_init(i);
        object_datum *object = (object_datum *)object_try_and_get_and_verify_type(i, object_mask_all);
        if (object)
            hcex_obj_customize(i, object->object.actor_variant_definition);
    }

    for (int j = data_next_index(light_data, -1); j != -1; j = data_next_index(light_data, j))
    {
        light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, j);
        if (object_try_and_get_and_verify_type(light->object_index, object_mask_all))
        {
            int attachment_index = (uint16_t)light->attachment_marker_index;
            if (attachment_index != 0xFFFF)
            {
                const char *marker_name = object_get_attachment_marker_name(light->object_index, attachment_index);
                if (marker_name && strcmp(marker_name, "flashlight") == 0)
                {
                    char *tag_name = tag_get_name(light->definition_index);
                    hcex_create_light(j, light->object_index, tag_name);
                }
            }
        }
    }

    int k = data_next_index(effect_data, -1);
    while (k != -1)
    {
        effect_datum *effect = effect_try_and_get(k);
        if (effect)
            hcex_init_effect_by_locations(k, effect->object_index, effect->scale_a);
        k = data_next_index(effect_data, k);
    }
}
