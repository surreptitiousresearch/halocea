/* ai_scripting_kill_internal @0x8376F7F8 — kill every actor addressed by an AI index, optionally silently
 * (no death reaction / dialogue). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void actor_kill(int actor_index, uint8_t silent, uint8_t delayed);

void ai_scripting_kill_internal(int ai_index, uint8_t silent)
{
    if ( ai_index == -1 )
        return;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
        actor_kill(iterator.iterator.index, silent, 0);
}
