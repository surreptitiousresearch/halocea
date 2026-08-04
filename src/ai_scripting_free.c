/* ai_scripting_free @0x83771AD8 — detach every actor of the addressed AI from its encounter, converting
 * them to free-roaming "encounterless" actors. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void actor_flush_position_indices(uint16_t actor_index);
extern void encounter_detach_actor(int actor_index, uint8_t died);
extern void encounterless_attach_actor(int actor_index);
extern void encounters_update_dirty_status(void);

void ai_scripting_free(int ai_index)
{
    if ( ai_index == -1 )
        return;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
    {
        actor_flush_position_indices(iterator.iterator.index);
        encounter_detach_actor(iterator.iterator.index, 0);
        encounterless_attach_actor(iterator.iterator.index);
    }
    encounters_update_dirty_status();
}
