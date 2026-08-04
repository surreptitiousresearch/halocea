/* mapcycle_init @ 0x837671C0 — allocate the (map,variant) cycle array */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"


void mapcycle_init(void)
{
    dynamic_array_new(&mapcycle_array, 8);  /* 8 bytes/element: {char *map, char *variant} */
}
