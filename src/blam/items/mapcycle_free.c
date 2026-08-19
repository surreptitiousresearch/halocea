/* mapcycle_free @ 0x83766228 — free each (map,variant) string pair, then the array */

#include "headers/dynamic_array.h"
#include "headers/mapcycle_item_s.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);

void mapcycle_free(void)
{
    int i;

    for ( i = 0; i < mapcycle_array.count; ++i )
    {
        /* the folded 8 was sizeof(mapcycle_item_s) — same typing mapcycle_add_item already uses */
        mapcycle_item_s *entry = (mapcycle_item_s *)mapcycle_array.elements + i;
        dlFree(entry->map_name);
        dlFree(entry->variant_name);
    }
    dynamic_array_delete(&mapcycle_array);
}
