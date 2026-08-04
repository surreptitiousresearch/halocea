/* ai_index_actor_iterator_next @0x8376F248 — advance the AI-index actor iterator, returning the next actor that
 * matches the squad/platoon restriction recorded at construction, or null at the end. Actors that fail the
 * squad or platoon filter are skipped. */

#include "headers/ai_index_actor_iterator.h"
#include "headers/actor_datum.h"

extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);

actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator)
{
    actor_datum *actor;
    while ( 1 )
    {
        actor = encounter_actor_iterator_next(&iterator->iterator);
        if ( !actor )
            break;
        if ( iterator->squad_index == -1 || iterator->squad_index == actor->meta.squad_index )
        {
            if ( iterator->platoon_index == -1 || iterator->platoon_index == actor->meta.platoon_index )
                break;
        }
    }
    return actor;
}
