/* object_type_update @ 0x83701C68 — fan the per-tick datum update out to every part definition of the
 * object's type (super-type plus sub-types), stopping at the null terminator. Returns 1 if any part's
 * datum_update reported a change. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


int object_type_update(int object_index)
{
    int result = 0;
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type_definition = object_type_definitions[object->object.type];

    for ( int i = 0; type_definition->part_definitions[i]; ++i )
    {
        unsigned __int8 (*datum_update)(int) = type_definition->part_definitions[i]->datum_update;
        if ( datum_update && datum_update(object_index) )
            result = 1;
    }

    return result;
}
