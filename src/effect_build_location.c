/* effect_build_location @0x836E1728 — allocate one effect-location datum for a single resolved marker
 * and link it into the effect's per-location instance list (singular counterpart of
 * effect_build_locations). Stores the marker's node index (with the 0x8000 first-person bit set or
 * cleared per `first_person_weapon`) and its node transform. Returns the new datum index, or -1 if the
 * location pool is full. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_location_datum.h"
#include "headers/object_marker.h"
#include "headers/blam_data_globals.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);

extern int datum_new(data_array *data);

int effect_build_location(effect_datum *effect, const object_marker *marker, int16_t location_index, uint8_t first_person_weapon)
{
    int datum_index = datum_new(effect_location_data);
    __int16 node_index = marker->node_index;
    effect_location_datum *location;

    if ( datum_index == -1 )
        return datum_index;

    location = DATUM_GET(effect_location_data, effect_location_datum, datum_index);
    if ( marker->node_index != -1 )
    {
        if ( first_person_weapon )
            node_index = marker->node_index | 0x8000;
        else
            node_index &= ~0x8000u;
    }
    location->node_designator = node_index;
    memcpy(&location->matrix, &marker->node_matrix, 0x34u);
    location->next_instance_location_index = effect->location_datum_indices[location_index];
    effect->location_datum_indices[location_index] = datum_index;

    return datum_index;
}
