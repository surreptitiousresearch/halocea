/* encounter_actor_iterator_prev @0x83709AC0 — move the cursor to the actor preceding the current one in its
 * encounter's actor chain, returning that actor's datum (or null if the current actor is already first, or if
 * it isn't found in the chain at all). Walks the chain from its head looking for the node whose
 * meta.next_actor_index equals the current index.
 *
 * Deviation: unlike encounter_actor_iterator_new.c, this always reads the chain head from encounter_data (never
 * ai_globals->first_encounterless_actor_index for encounter_index == -1) — confirmed via disasm, not just the
 * decompiler; this asymmetry is a property of the original code, not a translation error. */

#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/blam_data_globals.h"


actor_datum * encounter_actor_iterator_prev(encounter_actor_iterator *iterator)
{
    actor_datum *result = 0;

    if ( ai_globals->ai_initialized_for_map )
    {
        int target_index = iterator->index;
        int predecessor_index = -1;
        int cursor = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, iterator->encounter_index)->first_actor_index;

        if ( cursor != target_index )
        {
            while ( cursor != -1 )
            {
                predecessor_index = cursor;
                result = DATA_ARRAY_ELEMENT(actor_data, actor_datum, cursor);
                cursor = result->meta.next_actor_index;
                if ( cursor == target_index )
                    break;
            }
        }

        if ( cursor != target_index )
            return 0;

        iterator->next_index = cursor;
        iterator->index = predecessor_index;
    }

    return result;
}
