/* object_list_get_first @0x837757E8 — begin iterating an object list. Returns the first object index in the
 * list (or -1 if the list handle is invalid or empty) and writes the cursor (current reference index) into
 * *reference_index for a subsequent object_list_get_next call.
 *
 * The list header is a typed object_list_header_datum (first_reference_index at +8). object_list_data is a
 * datum array of data_reference elements (referenced object index at +4, next link at +8). */

#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/data_reference.h"
#include "headers/blam_data_globals.h"


int object_list_get_first(int object_list_index, int *reference_index)
{
    if ( object_list_index == -1 )
        return -1;

    object_list_header_datum *list =
        DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, object_list_index);
    int first_reference = list->first_reference_index;
    *reference_index = first_reference;
    if ( first_reference == -1 )
        return -1;

    /* recovered: raw 4*(u16)i + 4*((2i)&0x1FFFE) is 12*(u16)i, i.e. the data_reference element index */
    data_reference *reference = DATA_ARRAY_ELEMENT(object_list_data, data_reference, first_reference);
    *reference_index = reference->next_reference_index;   /* advance cursor to next reference */
    return reference->datum_index;                        /* object index */
}
