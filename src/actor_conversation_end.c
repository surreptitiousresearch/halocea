/* actor_conversation_end @0x837FCE08 — end an actor's AI conversation, if one is active (the actor's
 * external_orders.conversation_index). Same logic as action_converse_end; a distinct dispatch target the
 * compiler emitted as a separate function body. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);

void actor_conversation_end(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int conversation_index = actor->external_orders.conversation_index;
    if ( conversation_index != -1 )
        ai_conversation_finish(conversation_index, 0, 0);
}
