/* encounter_detach_actor @0x83709610 — remove an actor from its encounter's actor list (encounter +0x14 head,
 * actor +0x2C next link). Unless the actor died, also decrements the live counts up the hierarchy: the
 * encounter's actor count (+0x18), its live-actor count (+0x1C) if attached, the owning squad's original_count,
 * and the platoon's original_count. Clears the actor's encounter/squad/platoon references and marks the
 * encounter dirty (+0x28). No-op if AI isn't initialised. */

#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void encounter_detach_actor(int actor_index, uint8_t died)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.encounter_index == -1 )
        return;

    encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum, actor->meta.encounter_index);
    int *link = &encounter->first_actor_index;
    int current = encounter->first_actor_index;
    if ( current != actor_index )
    {
        do
        {
            actor_datum *cur = DATA_ARRAY_ELEMENT(actor_data, actor_datum, current);
            link = &cur->meta.next_actor_index;
            current = *link;
        }
        while ( *link != actor_index );
    }
    *link = actor->meta.next_actor_index;

    if ( !died )
    {
        /* DEVIATION: the squad-array index comes from actor+0x3A (meta.squad_index, meta 0x36) — the
         * decompiler's raw *((_WORD *)actor + 29) — not from meta.disconnected_squad_index at actor+0x38.
         * Must be read before the clear below, which writes -1 to that same halfword (0x83709738). */
        int16_t squad_array_index = actor->meta.squad_index + encounter->squad_base;
        --encounter->original_count;
        if ( actor->meta.unique_leader )
            --encounter->unique_leader_count;
        --squad_array[squad_array_index].original_count;
        int platoon_index = (uint16_t)actor->meta.platoon_index;
        if ( platoon_index != 0xFFFF )
            --platoon_array[(int16_t)(encounter->platoon_base + platoon_index)].original_count;
    }

    actor->meta.next_actor_index = -1;
    actor->meta.encounter_index = -1;
    actor->meta.platoon_index = -1;
    actor->meta.squad_index = -1;  /* sth -1, 0x3A(actor) — squad_index, not disconnected_squad_index (0x38) */
    encounter->status_dirty = 1;  /* encounter +0x28 dirty flag */
}
