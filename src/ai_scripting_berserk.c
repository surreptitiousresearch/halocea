/* ai_scripting_berserk @0x837727C8 — script command: set or clear the berserk flag on every actor selected by
 * the AI index. */

#include <stdint.h>
#include "headers/ai_index_actor_iterator.h"

#include "headers/actor_datum.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void actor_berserk(uint16_t actor_index, uint8_t berserk);

void ai_scripting_berserk(int ai_index, uint8_t berserk)
{
    if ( ai_index == -1 )
        return;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
        actor_berserk(iterator.iterator.index, berserk);
}
