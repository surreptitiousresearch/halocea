/* encounter_actor_iterator_next @0x83709A70 — advance the cursor and return the next actor datum, or null when
 * the chain is exhausted (or AI is not initialised for the map). Follows meta.next_actor_index links. */

#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/blam_data_globals.h"


actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator)
{
    actor_datum *result = 0;
    if ( ai_globals->ai_initialized_for_map )
    {
        int next_index = iterator->next_index;
        iterator->index = next_index;
        if ( next_index != -1 )
        {
            result = DATA_ARRAY_ELEMENT(actor_data, actor_datum, next_index);
            iterator->next_index = result->meta.next_actor_index;
        }
    }
    return result;
}
