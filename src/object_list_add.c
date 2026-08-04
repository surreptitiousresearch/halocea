/* object_list_add @0x837755D8 — append an object index to a list. Allocates a reference element from the
 * shared object_list_data pool (12-byte reference: object index at +4, next reference index at +8), links it at
 * the head of the list's chain (header first reference index at +8), and increments the list count (+6).
 * The count is incremented even when the reference pool is full and no element could be allocated. */

#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/data_reference.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);

void object_list_add(int object_list_index, int object_index)
{
    data_array *reference_pool = object_list_data;
    object_list_header_datum *list = DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, object_list_index);
    int reference_index = datum_new(object_list_data);
    if ( reference_index != -1 )
    {
        data_reference *reference = DATA_ARRAY_ELEMENT(reference_pool, data_reference, reference_index);
        reference->datum_index = object_index;
        reference->next_reference_index = list->first_reference_index;
        list->first_reference_index = reference_index;
    }
    ++list->count;
}
