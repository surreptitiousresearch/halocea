/* object_list_remove_reference @0x837756E0 — decrement the reference count of an object_list datum. Elements
 * are 12 bytes; the reference count is the word at offset 4. A -1 index is a no-op. */

#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/blam_data_globals.h"


void object_list_remove_reference(int object_list_index)
{
    if ( object_list_index != -1 )
    {
        object_list_header_datum *element =
            DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, object_list_index);
        --element->reference_count;
    }
}
