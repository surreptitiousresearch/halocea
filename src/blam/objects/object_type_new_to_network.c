/* object_type_new_to_network @0x837024D8 — serialize a newly created object into a network message
 * buffer by dispatching to the most-derived part definition's object_new_to_network callback. Scans
 * part_definitions from the deepest sub-type (index 15) up to the super-type and calls the first
 * callback found; returns the number of bits written, or 0 if the object is invalid or no part
 * provides a serializer.
 *
 * Object type selects the object_type_definition (object datum +180 = object.type). */

#include "headers/object_type_definition.h"
#include "headers/object_type.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int object_type_new_to_network(int object_index, void *buffer, int buffer_size_in_bits)
{
    object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_all);
    int i;

    if ( !object )
        return 0;

    for ( i = 15; i >= 0; --i )
    {
        object_type_definition *part = object_type_definitions[object->object.type]->part_definitions[i];
        if ( part && part->object_new_to_network )
            return part->object_new_to_network(object_index, buffer, buffer_size_in_bits);
    }
    return 0;
}
