/* actor_detach_from_unit @0x8371D778 — sever an actor from the (non-swarm) unit it controls: re-enable the
 * unit's automatic deactivation, stop actively controlling it, clear the unit's controlling-actor slot,
 * decrement the encounter's unique-leader count if this actor was one, and clear the actor's unit reference. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"

extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);

void actor_detach_from_unit(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int unit_index = actor->meta.unit_index;
    if ( unit_index != -1 )
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        object_set_automatic_deactivation(unit_index, 1u);
        unit_set_actively_controlled(actor->meta.unit_index, 0);
        unit->unit.actor_index = -1;
        if ( actor->meta.unique_leader )
        {
            if ( actor->meta.encounter_index != -1 )
            {
                encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum,
                                                                actor->meta.encounter_index);
                --encounter->unique_leader_count;
            }
        }
        actor->meta.unit_index = -1;
        actor->meta.unique_leader = 0;
    }
}
