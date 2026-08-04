/* object_add_to_list @ 0x836ED5A8 — Blam engine.
 * Pushes object_index onto the front of a singly-linked object list: stores the
 * current list head into the object's next_object_index link and makes the object
 * the new head. Uses the object-header datum-array index idiom. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"

void object_add_to_list(int *first_object_reference, int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object->object.next_object_index = *first_object_reference; /* was *(int *)(object + 276) */
    *first_object_reference = object_index;
}
