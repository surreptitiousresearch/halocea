/* ai_scripting_allow_dormant @0x837728B0 — script command: enable/disable dormancy for every squad selected by
 * an AI index. The squad disable_dormant flag is the inverse of the script's allow flag. */

#include <stdint.h>
#include "headers/ai_index_squad_iterator.h"
#include "headers/squad_datum.h"

extern void ai_index_squad_iterator_new(unsigned int ai_index, ai_index_squad_iterator *iterator);
extern squad_datum *ai_index_squad_iterator_next(ai_index_squad_iterator *iterator);

void ai_scripting_allow_dormant(int ai_index, uint8_t allow_dormant)
{
    ai_index_squad_iterator iterator;
    ai_index_squad_iterator_new(ai_index, &iterator);

    squad_datum *squad = ai_index_squad_iterator_next(&iterator);
    if ( squad )
    {
        unsigned char disable = allow_dormant == 0;
        do
        {
            squad->disable_dormant = disable;
            squad = ai_index_squad_iterator_next(&iterator);
        }
        while ( squad );
    }
}
