/* actor_set_team @0x8371D400 — propagate a team change to the unit(s) the actor controls. For a swarm actor it
 * writes the team to every member unit (either via the swarm_data member-unit list when a swarm cache exists, or
 * by walking the controlled-unit chain via object +0x1FC links); for a normal actor it writes the single
 * controlled unit's team. */

#include <stdint.h>

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


void actor_set_team(uint16_t actor_index, int16_t team_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.swarm )
    {
        if ( actor->meta.swarm_cache_index == -1 )  /* no swarm cache: walk controlled-unit chain */
        {
            for ( int unit_index = actor->meta.swarm_unit_index; unit_index != -1; )
            {
                unit_datum *unit =
                    (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                unit_index = unit->unit.swarm_next_unit_index;
                unit->object.owner_team_index = team_index;
            }
        }
        else
        {
            swarm_datum *swarm =
                DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
            for ( int i = 0; i < swarm->unit_count; ++i )
            {
                int member_unit_index = swarm->unit_indices[i];
                object_datum *unit =
                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, member_unit_index)->datum;
                unit->object.owner_team_index = team_index;
            }
        }
    }
    else
    {
        int unit_index = actor->meta.unit_index;
        if ( unit_index != -1 )
        {
            object_datum *unit =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
            unit->object.owner_team_index = team_index;
        }
    }
}
