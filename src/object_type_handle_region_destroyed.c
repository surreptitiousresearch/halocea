/* object_type_handle_region_destroyed @0x83701E80 — dispatch a region-destroyed event to every part
 * definition (super-type plus sub-types) registered for an object's type. The object's type code lives at
 * object datum +180. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


void object_type_handle_region_destroyed(int object_index, int region_index, unsigned int damage_region_flags)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];
    for ( int i = 0; type->part_definitions[i]; ++i )
    {
        void (*handle_region_destroyed)(int, int16_t, unsigned int) = type->part_definitions[i]->handle_region_destroyed;
        if ( handle_region_destroyed )
            handle_region_destroyed(object_index, region_index, damage_region_flags);
    }
}
