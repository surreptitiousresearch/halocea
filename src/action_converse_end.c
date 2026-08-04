/* action_converse_end @0x837FCE40 — end an actor's "converse" action state: finish the AI conversation
 * linked to this actor via its external_orders, if one is active. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);

void action_converse_end(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int conversation_index = actor->external_orders.conversation_index;
    if ( conversation_index != -1 )
        ai_conversation_finish(conversation_index, 0, 0);
}
