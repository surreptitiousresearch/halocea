/* object_list_new @0x837754F8 — allocate a new (empty) HaloScript object list. Returns the list datum index,
 * or -1 if the header pool is full. The 12-byte list header element: count (int16) at +6, first reference
 * index (int) at +8. */

#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);

int object_list_new(void)
{
    int list_index = datum_new(object_list_header_data);
    if ( list_index != -1 )
    {
        object_list_header_datum *list = DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, list_index);
        list->count = 0;
        list->first_reference_index = -1;
    }
    return list_index;
}
