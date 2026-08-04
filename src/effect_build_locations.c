/* effect_build_locations @0x836E2820 — resolve all marker locations for a spawned effect. For each
 * location in the effect definition, finds the named markers on the effect's object via the supplied
 * resolver and allocates an effect-location datum per marker (storing its node index and transform),
 * linking them into the effect's per-location list. First-person markers are tagged with the 0x8000
 * node-index bit. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/effect_definition.h"
#include "headers/effect_location_definition.h"
#include "headers/effect_location_datum.h"
#include "headers/object_marker.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>
extern void *memcpy(void *dst, const void *src, unsigned int size);


extern int datum_new(data_array *data);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void effect_build_locations(effect_datum *effect,
        int16_t (__fastcall *get_markers_by_name)(int, const char *, object_marker *, int16_t))
{
    effect_definition *definition = TAG_GET(effect_definition, effect->definition_index);
    int event;
    object_marker markers[16];   /* recovered: _DWORD markers[456] -> object_marker[16] */

    if ( definition->locations.count <= 0 )
        return;

    for ( event = 0; event < definition->locations.count; event = (__int16)(event + 1) )
    {
        int marker_count = get_markers_by_name(effect->object_index,
                ((effect_location_definition *)definition->locations.address)[event].marker_name, markers, 16);
        int i;

        if ( marker_count <= 0 )
            continue;

        for ( i = 0; i < marker_count; i = (__int16)(i + 1) )
        {
            object_marker *marker = &markers[i];   /* recovered: (__int16 *)&markers[27 * i] */
            int location_index = datum_new(effect_location_data);
            __int16 node_index = marker->node_index;
            effect_location_datum *location;

            if ( location_index == -1 )
                break;

            location = DATUM_GET(effect_location_data, effect_location_datum, location_index);
            if ( marker->node_index != -1 )
            {
                if ( get_markers_by_name == first_person_weapon_get_marker_by_name )
                    node_index = marker->node_index | 0x8000;
                else
                    node_index &= ~0x8000u;
            }
            location->node_designator = node_index;
            /* recovered: marker + 2 (int16*, byte +4) -> &marker->node_matrix */
            memcpy(&location->matrix, &marker->node_matrix, 0x34u);
            location->next_instance_location_index = effect->location_datum_indices[event];
            effect->location_datum_indices[event] = location_index;
        }
    }
}
