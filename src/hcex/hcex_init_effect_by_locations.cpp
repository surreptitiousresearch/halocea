/* hcex_init_effect_by_locations @0x836E0E00 — resolve every marker of a looping effect's definition on
 * its host object and hand the resulting positions/forwards (plus the marker names) to the ws-engine
 * bridge hcex_init_effect. Skips effects with no object. For each definition location marker, looks the
 * marker up either on a first-person weapon (when local_player_index != -1) or on the object, and on
 * success records the marker's object-space position (matrix row 3) and forward (matrix row 0). */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/global_tag_instances.h"
#include "../headers/effect_datum.h"
#include "../headers/effect_definition.h"
#include "../headers/effect_location_definition.h"
#include "../headers/object_marker.h"
#include "../headers/real_vector3d.h"
#include "../headers/real_point3d.h"

extern "C" data_array *effect_data;

extern "C" int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern "C" int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern "C" void hcex_init_effect(int definition_index, int obj_follow, int16_t local_player_index,
                             const real_point3d *positions, const real_vector3d *forwards,
                             const char **names, int count, double scale);

/* DEVIATION: arg0 was `uint16_t`, on the strength of the prologue's `clrlwi r11, r3, 16`. That mask
 * is DATA_ARRAY_ELEMENT's own datum-handle low-word extraction, not a parameter narrowing — the
 * callers decide. All three (hcex_obj_collect @0x83682120, effect_new_looping @0x836E3EF0,
 * effect_new_from_object @0x836E4060) move an UNMASKED 32-bit function return into r3 with a plain
 * `mr`; the C ABI puts the conversion-to-parameter-type on the caller, and MSVC cannot assume the
 * high half of a call return is zero. effect_new_looping is decisive: it emits `clrlwi r10, r3, 16`
 * @0x836E3E2C where it genuinely needs the absolute index, and no mask at the call. The value is a
 * packed handle (BUILD_DATUM_INDEX: salt high, index low) from datum_new / data_next_index, so a
 * 16-bit parameter would also have silently dropped the salt. DB prototype agrees: int. */
extern "C" void hcex_init_effect_by_locations(int effect_index, int obj_follow, float scale)
{
    effect_datum *effect = DATUM_GET(effect_data, effect_datum, effect_index);
    int found = 0;
    effect_definition *definition = *(effect_definition **)TAG_INSTANCE(effect->definition_index);

    if ( effect->object_index == -1 )
        return;

    real_vector3d forwards[32];
    real_point3d positions[32];
    const char *names[32];
    const char **name_out = names - 1;
    real_vector3d *forward_out = forwards;
    real_point3d *position_out = positions;

    if ( definition->locations.count > 0 )
    {
        int marker_index = 0;
        do
        {
            const char *marker_name =
                ((effect_location_definition *)definition->locations.address)[marker_index].marker_name;
            int object_index = effect->object_index;
            object_marker marker;
            int matched;

            if ( (uint16_t)effect->local_player_index == 0xFFFF )
                matched = object_get_marker_by_name(object_index, marker_name, &marker, 1);
            else
                matched = first_person_weapon_get_marker_by_name(object_index, marker_name, &marker, 1);

            if ( matched )
            {
                *++name_out = marker_name;
                ++found;
                forward_out->n[0] = marker.matrix.n[0][0];
                forward_out->n[1] = marker.matrix.n[0][1];
                forward_out->n[2] = marker.matrix.n[0][2];
                position_out->n[0] = marker.matrix.n[3][0];
                position_out->n[1] = marker.matrix.n[3][1];
                position_out->n[2] = marker.matrix.n[3][2];
                ++forward_out;
                ++position_out;
            }
            marker_index = (int16_t)(marker_index + 1);
        }
        while ( marker_index < definition->locations.count );
    }

    hcex_init_effect(effect->definition_index, obj_follow, effect->local_player_index,
                     positions, forwards, names, found, scale);
}
