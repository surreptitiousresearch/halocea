/* ai_conversation_finish @0x837CCF80 — tear down a running AI conversation. Records it in the ai_globals
 * recent-conversation ring buffer (16 entries, 16 bytes each: definition_index +0, unable_to_begin +2,
 * finished_successfully +3, finish_time +4) so the same conversation isn't immediately re-triggered, clears
 * each participant actor's conversation links, then frees the conversation datum.
 *
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/conversation_datum.h"
#include "headers/ai_conversation.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern void datum_delete(data_array *data, int index);

void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success)
{
    if ( conversation_index == -1 )
        return;

    conversation_datum *conversation =
        DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, conversation_index);
    ai_conversation *definition = &((ai_conversation *)global_scenario->ai_conversations.address)
                                    [conversation->conversation_definition_index];

    int slot = ai_globals->recent_conversation_next_index;
    int new_count = slot + 1;
    ai_globals->recent_conversation_next_index = (slot + 1) % 16;
    if ( ai_globals->recent_conversation_count > slot + 1 )
        new_count = ai_globals->recent_conversation_count;
    ai_globals->recent_conversation_count = new_count;

    /* recovered: &recent_conversations[16*slot] byte-punning -> typed recent_conversation[slot] */
    recent_conversation *entry = &ai_globals->recent_conversations[slot];
    entry->definition_index = conversation->conversation_definition_index;
    entry->unable_to_begin = unable_to_begin;
    entry->finished_successfully = success;
    entry->finish_time = game_time_get();

    int participant_count = definition->participants.count;
    for ( int participant = 0; participant < participant_count; participant = (__int16)(participant + 1) )
    {
        if ( ((1 << participant) & conversation->participant_bitmask) != 0
          && conversation->actor_indices[participant] != -1 )
        {
            actor_datum *actor = &((actor_datum *)actor_data->data)
                                   [(unsigned __int16)conversation->actor_indices[participant]];
            int actor_state = (unsigned __int16)actor->state.action;
            actor->external_orders.conversation_index = -1;
            actor->external_orders.conversation_attention_unit_index = -1;
            if ( actor_state == actor_action_converse )
                actor->state.action_data.___u0.converse.conversation_index = -1;
        }
    }

    datum_delete(conversation_data, conversation_index);
}
