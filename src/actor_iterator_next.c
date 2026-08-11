/* actor_iterator_next @0x83709BD0 — advance the actor cursor. Walks the next-actor chain of the current
 * encounter; when exhausted, moves to the next (optionally active) encounter, and finally the encounterless list.
 * With active_only set, skips inactive encounters and inactive actors. Returns the next matching actor, or null. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"

/* data_iterator_next is declared by data_iterator.h with the DB prototype
 * (void *__fastcall ...); the stale local extern was removed to fix C2371. */

actor_datum *actor_iterator_next(actor_iterator *iterator)
{
    if ( !ai_globals->ai_initialized_for_map )
        return 0;

    if ( iterator->next_index == -1 )
    {
        encounter_datum *encounter;
        while ( (encounter = data_iterator_next(&iterator->encounter_iterator)) != 0 )
        {
            if ( !iterator->active_only || encounter->active )
                iterator->next_index = encounter->first_actor_index;
            if ( iterator->next_index != -1 )
                goto walk_chain;
        }
        if ( !iterator->iterated_encounterless_list )
        {
            iterator->iterated_encounterless_list = 1;
            iterator->next_index = ai_globals->first_encounterless_actor_index;
        }
    }

walk_chain:
    while ( 1 )
    {
        int next_index = iterator->next_index;
        iterator->index = next_index;
        if ( next_index == -1 )
            break;
        uint8_t active_only = iterator->active_only;
        actor_datum *result = DATA_ARRAY_ELEMENT(actor_data, actor_datum, next_index);
        iterator->next_index = result->meta.next_actor_index;
        if ( !active_only || result->meta.active )
            return result;
    }
    return 0;
}
