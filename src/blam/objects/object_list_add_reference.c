/* object_list_add_reference @0x837756A8 — increment the reference count of an object_list datum.
 * A -1 index is a no-op. */

#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/blam_data_globals.h"


void object_list_add_reference(int object_list_index)
{
    if ( object_list_index != -1 )
    {
        object_list_header_datum *element = DATA_ARRAY_ELEMENT(object_list_header_data,
                          object_list_header_datum, object_list_index);
        ++element->reference_count;
    }
}
