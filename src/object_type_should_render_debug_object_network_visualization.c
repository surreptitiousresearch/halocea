/* object_type_should_render_debug_object_network_visualization @0x83702350 — object-type dispatch: OR of
 * the should_render_debug_object_network_visualization callbacks over the object's type-definition part
 * chain (null-terminated part_definitions list; the object's type word lives at object+180). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_type_definition.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


uint8_t object_type_should_render_debug_object_network_visualization(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];

    uint8_t should_render = 0;
    for ( object_type_definition **part = type->part_definitions; *part; part++ )
    {
        uint8_t (*callback)(int) = (*part)->should_render_debug_object_network_visualization;
        if ( callback )
            should_render = should_render || callback(object_index);
    }
    return should_render;
}
