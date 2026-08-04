/* ai_scripting_magically_see_encounter @0x83772910 — make every actor addressed by ai_index instantly aware
 * of every unit belonging to target_ai_index (using each target actor's controlled unit, or its swarm unit
 * if it has no individual unit). */

#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void ai_scripting_magically_see_unit(int ai_index, int unit_index);

void ai_scripting_magically_see_encounter(int ai_index, int target_ai_index)
{
    if ( ai_index == -1 || target_ai_index == -1 )
        return;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(target_ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        int unit_index = actor->meta.unit_index;
        if ( unit_index == -1 )
        {
            unit_index = actor->meta.swarm_unit_index;
            if ( unit_index == -1 )
                continue;
        }
        ai_scripting_magically_see_unit(ai_index, unit_index);
    }
}
