/* actor_attach_unit @0x83720C00 — bind an actor to a unit, evicting any prior occupant. Detaches any swarm or
 * normal actor currently controlling the unit, links the actor<->unit references, joins the actor's encounter,
 * copies the team, bumps the encounter's living-unit count for high-health units, then activates/deactivates and
 * marks the unit actively controlled. No-op if the actor already controls the unit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


extern void actor_swarm_detach_from_unit(int actor_index, int unit_index);
extern void actor_delete(int actor_index, uint8_t died);
extern void actor_detach_from_unit(uint16_t actor_index);
extern void encounter_attach_unit(int encounter_index, int unit_index);
extern void actor_input_update(int actor_index);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);
extern void object_deactivate(int object_index);
extern void object_activate(int object_index);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);

void actor_attach_unit(int actor_index, int unit_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    if ( unit->unit.actor_index == actor_index )
        return;

    int swarm_actor = unit->unit.swarm_actor_index;
    if ( swarm_actor != -1 )
        actor_swarm_detach_from_unit(swarm_actor, unit_index);
    int prior_actor = unit->unit.actor_index;
    if ( prior_actor != -1 )
        actor_delete(prior_actor, 0);
    if ( actor->meta.unit_index != -1 )
        actor_detach_from_unit(actor_index);

    actor->meta.unit_index = unit_index;
    unit->unit.actor_index = actor_index;

    int encounter_index = actor->meta.encounter_index;
    if ( encounter_index != -1 )
    {
        encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
        encounter_attach_unit(encounter_index, unit_index);
        unit->object.owner_team_index = encounter->team_index;
    }
    actor->meta.team_index = unit->object.owner_team_index;

    if ( unit->object.variant_number >= 100 )  /* DB member @186 is variant_number */
    {
        int actor_encounter = actor->meta.encounter_index;
        actor->meta.unique_leader = 1;
        if ( actor_encounter != -1 )
        {
            encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, actor_encounter);
            ++encounter->unique_leader_count;
        }
    }

    actor_input_update(actor_index);
    object_set_automatic_deactivation(unit_index, 0);
    if ( actor->meta.dormant )
        object_deactivate(unit_index);
    else
        object_activate(unit_index);
    unit_set_actively_controlled(unit_index, 1u);
}
