/* ai_scripting_force_active_by_unit @0x83771D18 — force the actor controlling the given unit active (or
 * release the force). Only applies to encounterless actors (those whose activation is otherwise managed by
 * their encounter). The actor index is read from the unit object at +500. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


void ai_scripting_force_active_by_unit(int unit_index, char active)
{
    if ( unit_index == -1 )
        return;

    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int actor_index = unit_object->unit.actor_index;
    if ( actor_index == -1 )
        return;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.encounterless )
        actor->meta.force_active = active;
}
