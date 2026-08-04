/* mapcycle_free @ 0x83766228 — free each (map,variant) string pair, then the array */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);

void mapcycle_free(void)
{
    int i;

    for ( i = 0; i < mapcycle_array.count; ++i )
    {
        void **entry = (void **)((char *)mapcycle_array.elements + i * 8);
        dlFree(entry[0]);   /* map name   */
        dlFree(entry[1]);   /* variant name */
    }
    dynamic_array_delete(&mapcycle_array);
}
