/* object_get_ultimate_parent @ 0x836EC690 — follow an object's parent chain to the root (the object
 * that is not itself attached to anything), or -1 for an invalid object. Object data +284 = parent
 * object index. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


int object_get_ultimate_parent(int object_index)
{
    int result = -1;
    int current = object_index;

    while ( current != -1 )
    {
        result = current;
        current = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current)->datum)->object.parent_object_index;
    }
    return result;
}
