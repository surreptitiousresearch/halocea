/* object_type_handle_parent_destroyed @0x83701F68 — notify every part definition (super-type plus
 * sub-types) registered for an object's type that the object's parent was destroyed. Returns 1 if any
 * part handler consumed the event, 0 otherwise. The object's type code lives at object datum +180. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


uint8_t object_type_handle_parent_destroyed(int object_index)
{
    int handled = 0;
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];
    for ( int i = 0; type->part_definitions[i]; ++i )
    {
        uint8_t (*handle_parent_destroyed)(int) = type->part_definitions[i]->handle_parent_destroyed;
        if ( handle_parent_destroyed && handle_parent_destroyed(object_index) )
            handled = 1;
    }
    return handled;
}
