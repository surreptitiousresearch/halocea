/* hcex_decals_precache_bitmaps @0x83710E00 — HCEX bridge hook: force every decal definition tag currently
 * referenced by a live decal instance to precache its bitmap_group's hardware textures (so decal rendering
 * never hitches on first use). Walks all live decals via a data_iterator. */

#include "../headers/data_array.h"
#include "../headers/data_iterator.h"
#include "../headers/decal_datum.h"
#include "../headers/decal_definition.h"
#include "../headers/global_tag_instances.h"

extern data_array *global_decal_data;

extern void hcex_load_bitmap_group(int bitmap_group_index);

extern "C" void hcex_decals_precache_bitmaps(void)
{
    if ( !global_decal_data->valid )
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, global_decal_data);

    while ( data_iterator_next(&iterator) )
    {
        decal_datum *decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, iterator.index);
        decal_definition *definition = *(decal_definition **)TAG_INSTANCE(decal->definition_index);
        int bitmap_group_index = definition->shader.decal.map.index;
        hcex_load_bitmap_group(bitmap_group_index);
    }
}
