/* ai_scripting_detach_unit @0x8376F6E0 — delete the actor currently controlling the given unit, detaching
 * the AI from it (the unit itself remains). The actor index is read from the unit object at +500. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_delete(int actor_index, uint8_t died);

void ai_scripting_detach_unit(int unit_index)
{
    if ( unit_index == -1 )
        return;

    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int actor_index = unit_object->unit.actor_index;
    if ( actor_index != -1 )
        actor_delete(actor_index, 0);
}
