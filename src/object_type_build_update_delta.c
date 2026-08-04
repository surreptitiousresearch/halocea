/* object_type_build_update_delta @0x83702654 — find the most-specific part definition (searching
 * part_definitions[15] down to [0]) that actually implements build_update_delta, and call it. Returns 0 if
 * none do. */

#include "headers/data_array.h"
#include "headers/object_type_definition.h"
#include "headers/object_header_datum.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/blam_data_globals.h"


int object_type_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *definition = object_type_definitions[object->object.type];

    for ( int i = 15; i >= 0; --i )
    {
        object_type_definition *part = definition->part_definitions[i];
        if ( part && part->build_update_delta )
            return part->build_update_delta(object_index, buffer, buffer_size_in_bits, mode);
    }

    return 0;
}
