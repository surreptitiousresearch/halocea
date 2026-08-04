/* prop_get_active_by_unit_index @0x837D1F10 — find an actor's active prop (knowledge record) for a given unit.
 * Walks the actor's prop list (meta.first_prop_index head, prop next_prop_index) for a perceived prop
 * (state >= 2) that either directly references the unit, or, if the prop is a swarm, references the unit's
 * controlling actor. Returns the prop index, or -1. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int controlling_actor = unit->unit.swarm_actor_index;
    if ( controlling_actor == -1 )
        controlling_actor = unit->unit.actor_index;

    int prop_index = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.first_prop_index;
    while ( prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int next = prop->next_prop_index;
        if ( (unsigned int)prop->state >= _prop_state_becoming_unacknowledged )
        {
            if ( prop->unit_index == unit_index )
                return prop_index;
            if ( prop->swarm )
            {
                int prop_actor = prop->actor_index;
                if ( prop_actor != -1 && prop_actor == controlling_actor )
                    return prop_index;
            }
        }
        prop_index = next;
    }
    return -1;
}
