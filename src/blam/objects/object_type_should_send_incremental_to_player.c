/* object_type_should_send_incremental_to_player @0x837029D0 — walks the object's type's part_definitions
 * chain from the most-derived type (index 15) down to the first entry with a non-null
 * should_send_incremental_to_player callback, and returns its result. Defaults to true (1) if no part in
 * the chain defines the callback. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


uint8_t object_type_should_send_incremental_to_player(int object_index, int player_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];

    for ( int16_t i = 15; i >= 0; --i )
    {
        object_type_definition *part = type->part_definitions[i];

        if ( part && part->should_send_incremental_to_player )
            return part->should_send_incremental_to_player(object_index, player_index);
    }

    return 1;
}
