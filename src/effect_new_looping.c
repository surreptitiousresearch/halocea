/* effect_new_looping @0x836E3DF8 — allocate a looping effect bound to an object. Allocates the effect
 * datum (looping flavor), wires its object/local-player/scale/change-color fields, clears the impulse
 * field, defaults the color to white when no change-color is supplied, marks the effect looping
 * (flag 0x2), builds its marker locations (object markers, plus first-person markers when the object
 * is a local first-person weapon), then initializes and runs a first update. Returns the effect
 * index, or -1 on allocation failure. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/object_marker.h"
#include "headers/object_marker.h"
#include "headers/data_array.h"
extern void *datum_get(data_array *data, int index);
extern int effect_allocate(int definition_index, int owner_object_index, uint8_t can_be_deterministic);
extern int16_t first_person_weapon_get_local_index(int object_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void effect_build_locations(effect_datum *effect, int16_t (*get_markers_by_name)(int, const char *, object_marker *, int16_t));
/* DEVIATION: arg0 was `uint16_t`. It is a 32-bit datum handle: all three call sites move an
 * unmasked function return (effect_allocate / data_next_index) into r3 with a plain `mr`, and a
 * 16-bit parameter would have forced the caller-side narrowing the C ABI requires. This very
 * function emits `clrlwi r10, r3, 16` @0x836E3E2C for its own use of the handle and does NOT
 * emit one at the call @0x836E3EF0. The callee's `clrlwi r11, r3, 16` is DATA_ARRAY_ELEMENT's
 * own low-word extraction, not a parameter narrowing. */
extern void hcex_init_effect_by_locations(int effect_index, int obj_follow, float scale);
extern void effect_update(int effect_index, float dt);

int effect_new_looping(int definition_index, int object_index, int16_t scale_a_function_index,
                       int16_t scale_b_function_index, int16_t change_color_index)
{
    int effect_index = effect_allocate(definition_index, object_index, 1u);
    if ( effect_index == -1 )
        return effect_index;

    /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
    effect_datum *effect = datum_get(effect_data, effect_index);
    effect->object_index = object_index;
    effect->local_player_index = first_person_weapon_get_local_index(object_index);
    effect->scale_a_function_index = scale_a_function_index;
    effect->scale_b_function_index = scale_b_function_index;
    effect->change_color_index = change_color_index;
    effect->impulse_field.translational = 0;
    effect->impulse_field.angular = 0;
    if ( change_color_index == -1 )
        effect->color = *global_real_rgb_white;
    effect->flags |= (1u << _effect_loop_bit);
    memset(effect->location_datum_indices, -1, sizeof(effect->location_datum_indices));

    effect_build_locations(effect, object_get_marker_by_name);
    if ( (uint16_t)effect->local_player_index != 0xFFFF )
        effect_build_locations(effect, first_person_weapon_get_marker_by_name);

    hcex_init_effect_by_locations(effect_index, object_index, 1.0);
    effect_update(effect_index, 0.0);

    return effect_index;
}
