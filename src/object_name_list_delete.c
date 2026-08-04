/* object_name_list_delete @0x836EE0D0 — detach an object from the scenario object-name table.
 *
 * If the object currently holds a name index, clears it and scans the object-name list, resetting any
 * slot that still points at this object to -1. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

void object_name_list_delete(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( (unsigned __int16)object->object.name_index != 0xFFFF )
    {
        object->object.name_index = -1;

        scenario *scenario = global_scenario;
        for ( __int16 i = 0; i < scenario->object_names.count; i = (__int16)(i + 1) )
        {
            if ( object_name_list[i] == object_index )
                object_name_list[i] = -1;
        }
    }
}
