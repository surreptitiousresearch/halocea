/* ai_scripting_stop_looking @0x837725C8 — stop the secondary "look at" behaviour of the actor controlling
 * the given unit. No-op if the unit has no controlling actor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_look_secondary_stop(uint16_t actor_index);

void ai_scripting_stop_looking(int unit_index)
{
    if ( unit_index == -1 )
        return;

    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int actor_index = unit_object->unit.actor_index;
    if ( actor_index != -1 )
        actor_look_secondary_stop(actor_index);
}
