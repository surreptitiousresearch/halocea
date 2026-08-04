/* object_type_adjust_placement @0x83701770 — run each part definition's datum_adjust_placement
 * callback (super-type then sub-types) for a freshly placed object, letting unit/weapon/etc. layers
 * fix up the placement data. The object's type definition is found via its type word (+180 in the
 * object data) into the object_type_definitions table.
 *
 * Object header datum (12-byte stride): object data pointer at dword[2] (+8).
 *
 * Deviation: the decompiler walks the part_definitions array via raw pointer arithmetic
 * (&v4->name + v6 + 32); reconstructed as the equivalent null-terminated array iteration. */

#include "headers/object_type_definition.h"
#include "headers/object_placement_data.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_type_adjust_placement(int object_index, object_placement_data *data)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = object_header->datum;
    object_type_definition *definition = object_type_definitions[object_data->object.type];
    int i;

    for ( i = 0; definition->part_definitions[i]; ++i )
    {
        void (*datum_adjust_placement)(int, struct object_placement_data *) =
            definition->part_definitions[i]->datum_adjust_placement;
        if ( datum_adjust_placement )
            datum_adjust_placement(object_index, data);
    }
}
