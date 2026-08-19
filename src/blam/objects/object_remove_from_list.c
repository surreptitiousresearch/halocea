/* object_remove_from_list @0x836ED5E0 — unlink an object from a singly-linked object list.
 *
 * Walks a list threaded through object.next_object_index starting at *first_object_reference; when the
 * target object is found it is spliced out (predecessor's link points past it) and its own next link
 * is cleared. No-op if the list is empty or the object is not present. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

void object_remove_from_list(int *first_object_reference, int object_index)
{
    if ( *first_object_reference == -1 )
        return;

    object_datum *object;
    int next_object_index;
    while ( 1 )
    {
        object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, *first_object_reference)->datum;
        next_object_index = object->object.next_object_index;
        if ( *first_object_reference == object_index )
            break;
        first_object_reference = &object->object.next_object_index;
        if ( next_object_index == -1 )
            return;
    }

    *first_object_reference = next_object_index;
    object->object.next_object_index = -1;
}
