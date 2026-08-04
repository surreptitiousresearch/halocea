/* ai_scripting_timer_start @0x8376FB58 — start the spawn-delay timer for every squad addressed by an AI
 * index (set its delay_timer_started flag). */

#include "headers/squad_datum.h"
#include "headers/ai_index_squad_iterator.h"

extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);

void ai_scripting_timer_start(int ai_index)
{
    if ( ai_index == -1 )
        return;

    ai_index_squad_iterator iterator;
    ai_index_squad_iterator_new(ai_index, &iterator);
    for ( squad_datum *squad = ai_index_squad_iterator_next(&iterator);
          squad;
          squad = ai_index_squad_iterator_next(&iterator) )
    {
        squad->delay_timer_started = 1;
    }
}
