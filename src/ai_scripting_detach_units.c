/* ai_scripting_detach_units @0x8376F728 — delete the controlling actor of every unit in the given object
 * list, detaching the AI from each (the units remain). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *iterator);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void actor_delete(int actor_index, uint8_t died);

void ai_scripting_detach_units(int object_list_index)
{
    int iterator[2];
    for ( int unit_index = object_list_get_first(object_list_index, iterator);
          unit_index != -1;
          unit_index = object_list_get_next(object_list_index, iterator) )
    {
        unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        int actor_index = unit_object->unit.actor_index;
        if ( actor_index != -1 )
            actor_delete(actor_index, 0);
    }
}
