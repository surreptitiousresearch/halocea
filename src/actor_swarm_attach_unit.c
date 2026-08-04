/* actor_swarm_attach_unit @0x83720E60 — add a unit to a swarm actor as an additional controlled body. Evicts any
 * actor already controlling the unit, links the unit onto the head of the actor's controlled-unit chain, sets up
 * its swarm component (when the actor has a swarm cache), copies team, bumps unit counts, joins the encounter,
 * and activates/deactivates the unit. Returns FALSE only if a needed swarm component could not be allocated. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void actor_swarm_detach_from_unit(uint16_t actor_index, int unit_index);
extern void actor_delete(int actor_index, uint8_t died);
extern void actor_detach_from_unit(uint16_t actor_index);
extern void actor_swarm_component_setup(uint16_t swarm_index, int unit_index, uint16_t swarm_component_index);
extern void encounter_attach_unit(int encounter_index, int unit_index);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);
extern void object_deactivate(int object_index);
extern void object_activate(int object_index);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);

uint8_t actor_swarm_attach_unit(int actor_index, int unit_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int succeeded = 1;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    if ( unit->unit.swarm_actor_index == actor_index )
        return succeeded;

    int component_index = -1;
    if ( actor->meta.swarm_cache_index != -1 )  /* actor has a swarm cache */
    {
        component_index = datum_new(swarm_component_data);
        succeeded = component_index != -1;
    }
    if ( !succeeded )
        return succeeded;

    int prior_swarm_actor = unit->unit.swarm_actor_index;
    if ( prior_swarm_actor != -1 )
        actor_swarm_detach_from_unit(prior_swarm_actor, unit_index);
    int prior_actor = unit->unit.actor_index;
    if ( prior_actor != -1 )
        actor_delete(prior_actor, 0);
    if ( actor->meta.unit_index != -1 )
        actor_detach_from_unit(actor_index);

    unit->unit.swarm_actor_index = actor_index;
    unit->unit.swarm_next_unit_index = actor->meta.swarm_unit_index;
    unit->unit.swarm_prev_unit_index = -1;
    if ( actor->meta.swarm_unit_index != -1 )
        (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.swarm_unit_index)->datum))
            ->unit.swarm_prev_unit_index = unit_index;

    int swarm_index = actor->meta.swarm_cache_index;
    actor->meta.swarm_unit_index = unit_index;
    if ( swarm_index != -1 )
        actor_swarm_component_setup(swarm_index, unit_index, component_index);

    __int16 controlled_unit_count = actor->meta.swarm_original_unit_count;
    int encounter_index = actor->meta.encounter_index;
    ++actor->meta.swarm_unit_count;
    actor->meta.swarm_original_unit_count = controlled_unit_count + 1;
    if ( encounter_index != -1 )
    {
        encounter_attach_unit(encounter_index, unit_index);
        unit->object.owner_team_index =
            DATUM_GET(encounter_data, encounter_datum, encounter_index)->team_index;
    }
    actor->meta.team_index = unit->object.owner_team_index;
    object_set_automatic_deactivation(unit_index, 0);
    if ( actor->meta.dormant )
        object_deactivate(unit_index);
    else
        object_activate(unit_index);
    unit_set_actively_controlled(unit_index, 1u);
    return succeeded;
}
