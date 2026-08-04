/* ai_scripting_braindead @0x83770FF8 — script command: set or clear the brain-dead flag on every actor selected
 * by the AI index. */

#include <stdint.h>
#include "headers/ai_index_actor_iterator.h"

#include "headers/actor_datum.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void actor_braindead(int actor_index, uint8_t braindead);

void ai_scripting_braindead(int ai_index, uint8_t braindead)
{
    if ( ai_index == -1 )
        return;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
        actor_braindead(iterator.iterator.index, braindead);
}
