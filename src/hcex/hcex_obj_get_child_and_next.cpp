/* hcex_obj_get_child_and_next @0x83683690 — read an object's first-child and next-sibling object ids
 * from its datum via the object-header data-array. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"

extern data_array *object_header_data;

extern "C" void hcex_obj_get_child_and_next(int id, int *idChild, int *idNext)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    *idChild = object->object.first_child_object_index;
    *idNext  = object->object.next_object_index;
}
