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

extern data_array *effect_data;

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void hcex_init_effect(int definition_index, int obj_follow, __int16 local_player_index,
                             const real_point3d *positions, const real_vector3d *forwards,
                             const char **names, int count, double scale);

extern "C" void hcex_init_effect_by_locations(unsigned __int16 effect_index, int obj_follow, float scale)
{
    effect_datum *effect = DATA_ARRAY_ELEMENT(effect_data, effect_datum, effect_index);
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

            if ( (unsigned __int16)effect->local_player_index == 0xFFFF )
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
            marker_index = (__int16)(marker_index + 1);
        }
        while ( marker_index < definition->locations.count );
    }

    hcex_init_effect(effect->definition_index, obj_follow, effect->local_player_index,
                     positions, forwards, names, found, scale);
}
