/* object_type_reset @0x83702158 — invoke the reset callback of every part definition (super-type plus
 * sub-types) registered for an object's type, walking the null-terminated part_definitions chain.
 * The object's type code lives at object datum +180. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


void object_type_reset(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];
    for ( int i = 0; type->part_definitions[i]; ++i )
    {
        void (*reset)(int) = type->part_definitions[i]->reset;
        if ( reset )
            reset(object_index);
    }
}
