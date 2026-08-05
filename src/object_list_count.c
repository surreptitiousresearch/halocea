/* object_list_count @0x837757B0 — return the number of objects in a HaloScript object list (object list
 * header +0x6), or 0 for an invalid list. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/blam_data_globals.h"


int object_list_count(int object_list_index)
{
    if ( object_list_index == -1 )
        return 0;
    object_list_header_datum *list =
        DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, object_list_index);
    return (uint16_t)list->count;
}
