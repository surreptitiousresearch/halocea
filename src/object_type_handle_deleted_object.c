/* object_type_handle_deleted_object @0x83701DD0 — notify `object_index`'s object-type chain (super-type
 * then sub-types via part_definitions[]) that `deleted_object_index` was deleted, so each part can drop any
 * reference it holds to it.
 *
 * DEVIATION: the decompiler advanced the chain via `(object_type_definition**)(&definition->name + v8)`
 * with v8 counting up from 33 — this is exactly `&definition->part_definitions[i]` in disguise
 * (part_definitions starts at pointer-index 32 relative to `name`); restored as plain array indexing. */

#include "headers/data_array.h"
#include "headers/object_type_definition.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_type_handle_deleted_object(int object_index, int deleted_object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *definition = object_type_definitions[object->object.type];

    if ( definition->part_definitions[0] )
    {
        for ( int i = 0; definition->part_definitions[i]; ++i )
        {
            void (*handle_deleted_object)(int, int) = definition->part_definitions[i]->handle_deleted_object;
            if ( handle_deleted_object )
                handle_deleted_object(object_index, deleted_object_index);
        }
    }
}
