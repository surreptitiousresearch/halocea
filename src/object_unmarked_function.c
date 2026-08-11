/* object_unmarked_function @0x836EDC90 — true when an object has NOT yet been marked for the current marker
 * generation (its mark word at object_data+20 differs from global_object_marker). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


int object_unmarked_function(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    return global_object_marker != object_data->object.magic_number;
}
