/* ai_scripting_timer_expire @0x8376FBC0 — immediately expire the spawn-delay timer of every squad addressed
 * by an AI index. */

#include <stdint.h>
#include "headers/squad_datum.h"
#include "headers/ai_index_squad_iterator.h"

extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);
extern void encounter_squad_timer_expire(uint16_t encounter_index, int16_t squad_index);

void ai_scripting_timer_expire(int ai_index)
{
    if ( ai_index == -1 )
        return;

    ai_index_squad_iterator iterator;
    ai_index_squad_iterator_new(ai_index, &iterator);
    while ( ai_index_squad_iterator_next(&iterator) )
        encounter_squad_timer_expire(iterator.encounter_index, iterator.squad_index);
}
