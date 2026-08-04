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
extern void *datum_get(data_array *array, int index);
extern int effect_allocate(int definition_index, int owner_object_index, uint8_t can_be_deterministic);
extern int16_t first_person_weapon_get_local_index(int object_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void effect_build_locations(effect_datum *effect, int16_t (__fastcall *get_markers_by_name)(int, const char *, object_marker *, int16_t));
extern void hcex_init_effect_by_locations(unsigned __int16 effect_index, int obj_follow, float scale);
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
    if ( (unsigned __int16)effect->local_player_index != 0xFFFF )
        effect_build_locations(effect, first_person_weapon_get_marker_by_name);

    hcex_init_effect_by_locations(effect_index, object_index, 1.0);
    effect_update(effect_index, 0.0);

    return effect_index;
}
