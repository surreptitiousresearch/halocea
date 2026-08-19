/* ai_scripting_automatic_migration_target @0x83772608 — script command: set the "automatic migration target" flag
 * (squad+0x10) on every squad selected by the AI index. */

#include <stdint.h>
#include "headers/squad_datum.h"
#include "headers/ai_index_squad_iterator.h"

extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);

void ai_scripting_automatic_migration_target(int ai_index, uint8_t enable)
{
    if ( ai_index == -1 )
        return;

    ai_index_squad_iterator iterator;
    ai_index_squad_iterator_new(ai_index, &iterator);
    for ( squad_datum *squad = ai_index_squad_iterator_next(&iterator);
          squad;
          squad = ai_index_squad_iterator_next(&iterator) )
        squad->automatic_migration_target = enable;
}
