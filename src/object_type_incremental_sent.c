/* object_type_incremental_sent @0x83702A58 — notify the object type's (and any chained super-type's)
 * incremental_sent callback that a network update for this object has been sent, walking the
 * part_definitions chain from the most-derived type (index 15) down to the first entry with a non-null
 * callback.
 *
 * DEVIATION: the decompiler declared `incremental_sent` as a zero-arg function pointer and called it with
 * no arguments; object_type_definition.h's own field type is `void (*incremental_sent)(int)`, and
 * disasm_range(0x83702A58,0x83702AE0) confirms r3 (object_index) is never reassigned before the `bctr` —
 * the classic bctr-tail-call arg-loss pattern. Called with `object_index` restored. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


void object_type_incremental_sent(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];

    for ( __int16 i = 15; i >= 0; --i )
    {
        object_type_definition *part = type->part_definitions[i];

        if ( part && part->incremental_sent )
        {
            part->incremental_sent(object_index);
            return;
        }
    }
}
