/* hcex_obj_get_function_in @0x83681C68 — read one of an object's animation/function output scalars.
 * Resolves the object datum from the object-header data-array, then reads the requested function slot from
 * the per-object incoming_function_values block. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"

extern "C" data_array *object_header_data;

extern "C" float hcex_obj_get_function_in(int object_index, int function_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    return object->object.incoming_function_values[function_index];
}
