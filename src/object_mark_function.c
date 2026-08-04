/* object_mark_function @0x836EDCC8 — mark an object for the current marker generation. Returns 0 if it was
 * already marked, else stamps the marker word (object_data+20) and returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


/* param respelled int->uint16_t 2026-07-30: disasm first-use clrlwi r10,r3,16; matches its three
 * visibility-callback siblings (object_unmarked_function/light_unmarked/light_mark, all uint16_t) */
int object_mark_function(uint16_t object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if (object_data->object.magic_number == global_object_marker)
        return 0;
    object_data->object.magic_number = global_object_marker;
    return 1;
}
