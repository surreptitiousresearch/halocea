/* effect_delete @ 0x836E0F58 — destroy a running effect and all of its per-location instances.
 * For each location in the effect definition it walks that location's instance list (heads in
 * effect->location_datum_indices, each chained through next_instance_location_index) and frees
 * every location datum, then frees the effect datum itself. No-op if the effect handle is already
 * stale. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/effect_definition.h"
#include "headers/effect_location_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern void datum_delete(data_array *data, int index);

void effect_delete(int effect_index)
{
    effect_datum *effect = datum_try_and_get(effect_data, effect_index);
    if ( effect )
    {
        effect_definition *definition = TAG_GET(effect_definition, effect->definition_index);
        if ( definition->locations.count > 0 )
        {
            __int16 location_slot = 0;
            do
            {
                for ( int location_index = effect->location_datum_indices[location_slot]; location_index != -1; )
                {
                    effect_location_datum *location = DATUM_GET(effect_location_data,
                            effect_location_datum, location_index);
                    /* DEVIATION: read the "next" link before freeing; the decompiler reads it from the
                     * just-freed datum after datum_delete. Reading first is the equivalent correct order. */
                    int next_location_index = location->next_instance_location_index;
                    datum_delete(effect_location_data, location_index);
                    location_index = next_location_index;
                }
                location_slot = (__int16)(location_slot + 1);
            }
            while ( location_slot < definition->locations.count );
        }
        datum_delete(effect_data, effect_index);
    }
}
