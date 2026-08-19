/* ai_scripting_allow_charge @0x837715F8 — script command: enable/disable charging for every actor selected by
 * an AI index. The external-orders disable_charging flag is the inverse of the script's allow flag. */

#include <stdint.h>
#include "headers/ai_index_actor_iterator.h"
#include "headers/actor_datum.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);

void ai_scripting_allow_charge(int ai_index, uint8_t allow_charge)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);

    actor_datum *actor = ai_index_actor_iterator_next(&iterator);
    if ( actor )
    {
        unsigned char disable = allow_charge == 0;
        do
        {
            actor->external_orders.disable_charging = disable;
            actor = ai_index_actor_iterator_next(&iterator);
        }
        while ( actor );
    }
}
