/* object_type_export_function_values @ 0x83701D28 — fan the per-tick "export function values" callback
 * out to every part definition of the object's type, stopping at the null terminator. */

#include "headers/data_array.h"
#include "headers/object_type_definition.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_type_export_function_values(int object_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_type_definition *type_definition = object_type_definitions[object_header->datum->object.type];

    for ( int i = 0; type_definition->part_definitions[i]; ++i )
    {
        void (*datum_export_function_values)(int) = type_definition->part_definitions[i]->datum_export_function_values;
        if ( datum_export_function_values )
            datum_export_function_values(object_index);
    }
}
