/* object_list_delete @0x83775560 — free an object list: delete the whole reference chain (the references live
 * in the shared object_list_data pool, chained from the header's first reference index at +8), then free the
 * list header datum itself. */

#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/blam_data_globals.h"

extern void reference_list_delete(data_array *array, int first_reference_index);
extern void datum_delete(data_array *data, int index);

void object_list_delete(int list_index)
{
    if ( list_index != -1 )
    {
        object_list_header_datum *list =
            DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, list_index);
        reference_list_delete(object_list_data, list->first_reference_index);
        datum_delete(object_list_header_data, list_index);
    }
}
