/* object_type_disconnect_from_structure_bsp @0x83702200 — fan the "disconnect from structure BSP" callback
 * out to every part definition of the object's type, stopping at the null terminator. Mirrors
 * object_type_export_function_values.c's part-definitions fan-out exactly, for a different callback slot.
 *
 * DEVIATION: the decompiler rendered the part-definitions walk as raw pointer arithmetic
 * (`&v2->name + v6`) advancing by a hand-computed index; restored as a plain indexed loop over the
 * part_definitions[16] array already modeled in object_type_definition.h. */

#include "headers/data_array.h"
#include "headers/object_type_definition.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_type_disconnect_from_structure_bsp(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type_definition = object_type_definitions[object_data->object.type];

    for ( int i = 0; type_definition->part_definitions[i]; ++i )
    {
        void (*disconnect_from_structure_bsp)(int) = type_definition->part_definitions[i]->disconnect_from_structure_bsp;
        if ( disconnect_from_structure_bsp )
            disconnect_from_structure_bsp(object_index);
    }
}
