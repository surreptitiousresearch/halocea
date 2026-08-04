/* object_try_and_get_and_verify_type @ 0x836EC540 — safely fetch an object's data pointer, but only if
 * the object exists and its type code (header byte[3]) is among the allowed set (1<<type tested against
 * valid_type_flags). Returns the object data pointer, or null if the handle is stale or the type is not
 * permitted. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags)
{
    object_header_datum *object_header = datum_try_and_get(object_header_data, object_index);
    if ( object_header )
    {
        if ( ((1 << object_header->type) & valid_type_flags) != 0 )
            return object_header->datum;
    }
    return 0;
}
