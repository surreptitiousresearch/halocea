/* object_get_type @0x83757B88 — return an object's type (the byte at +3 of its object-header element). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


int object_get_type(int object_index)
{
    return DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->type;
}
