/* recursive_object_adder @0x836EE1D8 — walk an object's attachment hierarchy (first child at
 * object+0x118, next sibling at object+0x114 — same fields documented in object_get_features_in_sphere.c
 * and unit_can_enter_seat.c), appending each object whose optional filter accepts it to `object_indices`
 * until `maximum_count` is reached.
 *
 * DEVIATION: the DB decompiler shows `add_object_function` as a zero-argument callback and never
 * references `custom_data`; disasm_range(0x836EE1D8, 0x836EE2AC) shows it's actually called as
 * `add_object_function(object_index, custom_data)` (r3, r4 at the call site). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


int recursive_object_adder(
        int object_index,
        unsigned __int8 (*add_object_function)(int object_index, void *custom_data),
        void *custom_data,
        int object_count,
        int maximum_count,
        int *object_indices)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if (object_count >= maximum_count)
        return object_count;

    if (!add_object_function || add_object_function(object_index, custom_data))
        object_indices[object_count++] = object_index;

    int first_child = object->object.first_child_object_index;
    if (first_child != -1)
        object_count = recursive_object_adder(first_child, add_object_function, custom_data, object_count,
                                               maximum_count, object_indices);

    int next_sibling = object->object.next_object_index;
    if (next_sibling == -1)
        return object_count;

    return recursive_object_adder(next_sibling, add_object_function, custom_data, object_count, maximum_count,
                                   object_indices);
}
