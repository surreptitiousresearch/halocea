/* object_name_list_new @0x836EE080 — bind an object to a scenario object-name slot.
 *
 * If the name slot is free (-1), records this object in it and stores the name index on the object. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

void object_name_list_new(int object_index, int16_t name_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( object_name_list[name_index] == -1 )
    {
        object_name_list[name_index] = object_index;
        object->object.name_index = name_index;
    }
}
