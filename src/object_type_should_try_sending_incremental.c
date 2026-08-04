/* object_type_should_try_sending_incremental @0x8370296C — walks the object's type's part_definitions chain
 * from the most-derived type (index 15) down to the first entry with a non-null should_try_sending_incremental
 * callback, and returns its result. Defaults to true (1) if no part in the chain defines the callback. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


uint8_t object_type_should_try_sending_incremental(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];

    for ( __int16 i = 15; i >= 0; --i )
    {
        object_type_definition *part = type->part_definitions[i];

        if ( part && part->should_try_sending_incremental )
            return part->should_try_sending_incremental(object_index);
    }

    return 1;
}
