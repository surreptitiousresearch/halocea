/* actor_delete @0x83720178 — fully tear down an actor: detach it from its encounter (or encounterless pool),
 * dismantle its swarm (cache + every member unit) or its single unit, delete its props, orphan any prop in the
 * pool that still references this actor, notify the conversation system, and free the actor datum. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern void encounterless_detach_actor(int actor_index);
extern void encounter_detach_actor(int actor_index, uint8_t died);
extern void actor_swarm_cache_delete(uint16_t actor_index);
extern void actor_swarm_detach_from_unit(int actor_index, int unit_index);
extern void actor_detach_from_unit(uint16_t actor_index);
extern void actor_delete_props(uint16_t actor_index);
extern void ai_conversation_actor_deleted(int actor_index);
extern void datum_delete(data_array *data, int index);

void actor_delete(int actor_index, uint8_t died)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.encounterless )
        encounterless_detach_actor(actor_index);
    else
        encounter_detach_actor(actor_index, died);

    if ( actor->meta.swarm )  /* swarm */
    {
        actor_swarm_cache_delete(actor_index);
        for ( int unit = actor->meta.swarm_unit_index; unit != -1; unit = actor->meta.swarm_unit_index )
            actor_swarm_detach_from_unit(actor_index, unit);
    }
    else
    {
        actor_detach_from_unit(actor_index);
    }

    actor_delete_props(actor_index);

    data_iterator iterator;
    data_iterator_new(&iterator, prop_data);
    for ( prop_datum *prop = data_iterator_next(&iterator); prop; prop = data_iterator_next(&iterator) )
    {
        if ( prop->actor_index == actor_index )  /* prop+0x1C */
            prop->actor_index = -1;
    }

    ai_conversation_actor_deleted(actor_index);
    datum_delete(actor_data, actor_index);
}
