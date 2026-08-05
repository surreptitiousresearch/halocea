/* ai_conversation_stop @0x837CE6E0 — forcibly end every running runtime conversation with the given
 * definition index. Each match is logged into ai_globals.recent_conversations (a 16-entry ring marked
 * neither "unable to begin" nor "finished successfully", with the current game time), its placed actors are
 * released from the converse action, and the conversation datum is deleted. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/scenario.h"
#include "headers/conversation_datum.h"
#include "headers/ai_conversation.h"
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void datum_delete(data_array *data, int index);
extern int game_time_get(void);

void ai_conversation_stop(int16_t conversation_definition_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, conversation_data);
    for ( conversation_datum *conversation = data_iterator_next(&iterator);
          conversation;
          conversation = data_iterator_next(&iterator) )
    {
        if ( conversation->conversation_definition_index != conversation_definition_index )
            continue;

        int index = iterator.index;
        if ( index == -1 )
            continue;

        /* the folded 116 was sizeof(ai_conversation) */
        ai_conversation *conversations = (ai_conversation *)global_scenario->ai_conversations.address;
        conversation_datum *conversation_at = DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, index);
        ai_conversation *definition = &conversations[conversation_at->conversation_definition_index];

        /* Append to the recent-conversations ring (size 16). */
        int recent_index = ai_globals->recent_conversation_next_index;
        int16_t recent_count = recent_index + 1;
        ai_globals->recent_conversation_next_index = recent_index + 1;
        ai_globals->recent_conversation_next_index -=
            16 * ((ai_globals->recent_conversation_next_index >> 4)
                  + (ai_globals->recent_conversation_next_index < 0
                     && (ai_globals->recent_conversation_next_index & 0xF) != 0));
        if ( ai_globals->recent_conversation_count > recent_index + 1 )
            recent_count = ai_globals->recent_conversation_count;
        ai_globals->recent_conversation_count = recent_count;
        ai_globals->recent_conversations[recent_index].definition_index = conversation_at->conversation_definition_index;
        ai_globals->recent_conversations[recent_index].unable_to_begin = 0;
        ai_globals->recent_conversations[recent_index].finished_successfully = 0;
        ai_globals->recent_conversations[recent_index].finish_time = game_time_get();

        /* Release placed actors from the converse action. */
        if ( definition->participants.count > 0 )
        {
            for ( int slot = 0; slot < definition->participants.count; slot = (int16_t)(slot + 1) )
            {
                if ( ((1 << slot) & conversation_at->participant_bitmask) != 0
                  && conversation_at->actor_indices[slot] != -1 )
                {
                    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, conversation_at->actor_indices[slot]);
                    int current_action = actor->state.action;                    /* actor +0x6C */
                    actor->external_orders.conversation_index = -1;              /* actor +0x1DC — conversation refs */
                    actor->external_orders.conversation_attention_unit_index = -1; /* actor +0x1E0 */
                    if ( current_action == actor_action_converse )
                        actor->state.action_data.___u0.converse.conversation_index = -1; /* actor +0x9C — converse action data */
                }
            }
        }

        datum_delete(conversation_data, index);
    }
}
