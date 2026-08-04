/* ai_scripting_look_at_object @0x83772508 — make the actor controlling the given unit look toward an object
 * (as a secondary look, priority 1, action type 13). The look direction is an empty direction spec retyped
 * to "look at prop" (type 6) targeting the object. No-op if the unit has no controlling actor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/direction_specification.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/direction_specification_type.h"
#include "headers/blam_data_globals.h"

extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);

void ai_scripting_look_at_object(int unit_index, int object_index)
{
    if ( unit_index == -1 || object_index == -1 )
        return;

    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int actor_index = unit_object->unit.actor_index;
    if ( actor_index == -1 )
        return;

    direction_specification empty;
    direction_specification look_direction = *direction_get_empty(&empty);
    look_direction.type = _direction_specification_object;
    look_direction.___u1.prop_index = object_index;
    actor_look_secondary(actor_index, _secondary_look_scripted, _secondary_look_priority_default, &look_direction);
}
