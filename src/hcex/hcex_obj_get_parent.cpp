/* hcex_obj_get_parent @0x836836C8 — report an object's parent attachment. Resolves the object datum via the
 * object-header data-array, reads the parent object index; if it is -1 the object is unparented and this
 * returns 0. Otherwise the parent object index and the parent bone/node index (a signed byte) are written to
 * the out params and it returns 1. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"

extern data_array *object_header_data;

extern "C" int hcex_obj_get_parent(int id, int *idParent, int *idParentBone)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    int parent_index = object->object.parent_object_index;
    if (parent_index == -1)
        return 0;
    *idParent = parent_index;
    *idParentBone = object->object.parent_node_index;
    return 1;
}
