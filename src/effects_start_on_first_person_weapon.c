/* effects_start_on_first_person_weapon @0x836E35F0 — attach a first-person weapon's effects to the local
 * player's viewmodel markers. Scans the effect data array for effects attached to `object_index`; for
 * each, marks it owned by `local_player_index` and, for every location in the effect definition's
 * locations block (32-byte marker-name records), resolves the matching first-person weapon markers (up to
 * 16) and creates an effect_location datum per marker. Each new location stores the (possibly attached,
 * bit 0x8000) node designator and node matrix, and is pushed onto the effect's per-location
 * singly-linked list (location_datum_indices heads). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/effect_datum.h"
#include "headers/effect_location_datum.h"
#include "headers/effect_definition.h"
#include "headers/effect_location_definition.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern int datum_new(data_array *data);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void effects_start_on_first_person_weapon(int16_t local_player_index, int object_index)
{
    for ( int i = data_next_index(effect_data, -1); i != -1; i = data_next_index(effect_data, i) )
    {
        effect_datum *effect = DATA_ARRAY_ELEMENT(effect_data, effect_datum, i);
        if ( effect->object_index != object_index )
            continue;

        effect->local_player_index = local_player_index;
        effect_definition *definition =
            TAG_GET(effect_definition, effect->definition_index);
        if ( definition->locations.count <= 0 )
            continue;

        object_marker markers[17];
        int location_index = 0;
        do
        {
            int marker_count = first_person_weapon_get_marker_by_name(
                effect->object_index,
                ((effect_location_definition *)definition->locations.address)[location_index].marker_name,
                markers, 16);

            if ( marker_count > 0 )
            {
                int m = 0;
                do
                {
                    object_marker *marker = &markers[m];
                    int new_location = datum_new(effect_location_data);
                    if ( new_location == -1 )
                        break;

                    int16_t node_index = marker->node_index;
                    effect_location_datum *location =
                        DATA_ARRAY_ELEMENT(effect_location_data, effect_location_datum, new_location);
                    if ( marker->node_index != -1 )
                        node_index = marker->node_index | 0x8000;
                    location->node_designator = node_index;
                    memcpy(&location->matrix, &marker->node_matrix, 0x34u);

                    m = (int16_t)(m + 1);
                    location->next_instance_location_index =
                        effect->location_datum_indices[location_index];      /* next = old head */
                    effect->location_datum_indices[location_index] = new_location; /* head = new */
                }
                while ( m < marker_count );
            }
            location_index = (int16_t)(location_index + 1);
        }
        while ( location_index < definition->locations.count );
    }
}
