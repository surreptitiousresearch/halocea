/* object_type_render_debug 0x837022F0 — fan out a debug-render call to every part definition in the object's
 * type chain. Indexes object_type_definitions by the object's type code, then walks the null-terminated
 * part_definitions[] list, invoking each part's render_debug(object_index) callback when present. */

#include <stdint.h>
#include "headers/object_type_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_type_render_debug(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type_definition = object_type_definitions[object_data->object.type];

    for ( int16_t part_index = 0;
          type_definition->part_definitions[part_index];
          part_index = (short)(part_index + 1) )
    {
        void (*render_debug)(int) = type_definition->part_definitions[part_index]->render_debug;
        if ( render_debug )
            render_debug(object_index);
    }
}
