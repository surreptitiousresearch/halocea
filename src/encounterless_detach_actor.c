/* encounterless_detach_actor @0x83709850 — remove an actor from the singly-linked list of encounterless
 * actors rooted at ai_globals->first_encounterless_actor_index (next link at actor +0x2C / dword[11]).
 * Clears the actor's meta.encounterless flag (+0x09) and meta.force_active flag (+0x0A) — the latter
 * was previously mislabeled "active flag"; DB says force_active. No-op if AI isn't initialised. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void encounterless_detach_actor(int actor_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    int *link = &ai_globals->first_encounterless_actor_index;
    int current = ai_globals->first_encounterless_actor_index;
    actor_datum *data = (actor_datum *)actor_data->data;
    actor_datum *actor = &data[(uint16_t)actor_index];

    if ( current != actor_index )
    {
        do
        {
            link = &data[(uint16_t)current].meta.next_actor_index;  /* actor +0x2C next */
            current = *link;
        }
        while ( *link != actor_index );
    }
    *link = actor->meta.next_actor_index;
    actor->meta.encounterless = 0;
    actor->meta.next_actor_index = -1;
    actor->meta.force_active = 0;
}
