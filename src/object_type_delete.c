/* object_type_delete @0x83701BB8 — tear down an object's per-type state: first the HCEX bridge
 * (hcex_obj_term), then each part definition's datum_delete callback (super-type then sub-types).
 *
 * Deviation: the decompiler walks part_definitions via raw pointer arithmetic; reconstructed as
 * null-terminated array iteration. */

#include "headers/object_type_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern void hcex_obj_term(int object_index);

void object_type_delete(int object_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_type_definition *definition = object_type_definitions[object_header->datum->object.type];
    int i;

    hcex_obj_term(object_index);

    for ( i = 0; definition->part_definitions[i]; ++i )
    {
        void (*datum_delete)(int) = definition->part_definitions[i]->datum_delete;
        if ( datum_delete )
            datum_delete(object_index);
    }
}
