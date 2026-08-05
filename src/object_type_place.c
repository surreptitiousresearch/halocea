/* object_type_place @0x837018E8 — run the per-part datum_place callbacks for a freshly created scenario object.
 * The object's type is read from its runtime object data (+180), and each non-null part definition in that
 * type's chain whose datum_place is set is invoked with the scenario placement datum.
 *
 * Deviation: the decompiler advances the part-definition pointer with raw arithmetic (&name + (i+32));
 * reproduced as plain part_definitions[i] indexing (offset 0x80 = 32 pointers from the struct base). */

#include <stdint.h>
#include "headers/object_type_definition.h"
#include "headers/scenario_object_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


void object_type_place(int object_index, scenario_object_datum *scenario_object)
{
    object_datum *object_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object_data->object.type];

    for ( int16_t i = 0; type->part_definitions[i]; ++i )
    {
        void (*datum_place)(int, void *) = type->part_definitions[i]->datum_place;
        if ( datum_place )
            datum_place(object_index, scenario_object);
    }
}
