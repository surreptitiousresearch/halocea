/* mapcycle_add_item @0x83766CE0 — append a (copied) map/variant name pair to the mapcycle array. */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include "headers/mapcycle_item_s.h"
#include "headers/blam_data_globals.h"

extern void * copy_str(const char *str);

uint8_t mapcycle_add_item(const char *map_name, const char *variant_name)
{
    int index = dynamic_array_add_element(&mapcycle_array);

    if ( index == -1 )
        return 0;

    mapcycle_item_s *item = (mapcycle_item_s *)mapcycle_array.elements + index;
    item->map_name = copy_str(map_name);
    item->variant_name = copy_str(variant_name);
    return 1;
}
