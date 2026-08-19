/* _rasterizer_decals_initialize_for_new_map @0x83710EA0 — clear the ws-engine decal bridge and re-create a
 * bridged decal for every valid Blam decal currently in global_decal_data. (The public wrapper
 * rasterizer_decals_initialize_for_new_map skips this when running the import tool.)
 * Return type is void: the r3 at blr is only the loop-exit (null) iterator residue with no caller
 * consumers. */

#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/real_point3d.h"
#include "headers/decal_datum.h"
#include "headers/blam_data_globals.h"

extern void hcex_decals_clear(void);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void hcex_init_decal(int decal_definition_index, const real_point3d *point);

void _rasterizer_decals_initialize_for_new_map(void)
{
    void *result;
    hcex_decals_clear();
    if ( global_decal_data->valid )
    {
        data_iterator iterator;
        data_iterator_new(&iterator, global_decal_data);
        for ( result = data_iterator_next(&iterator); result; result = data_iterator_next(&iterator) )
        {
            decal_datum *decal = DATUM_GET(global_decal_data, decal_datum, iterator.index);
            hcex_init_decal(decal->definition_index, &decal->position);
        }
    }
}
