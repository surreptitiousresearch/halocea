/* ai_conversation_unit_died @0x837CEAF0 — notify the AI conversation system that a unit has died (or been
 * deleted). Walks every active conversation, clearing references to the dead unit from the line-in-progress
 * fields and (when the unit was deleted) from participant actors' remembered slots. A conversation that both
 * referenced the unit and whose definition is non-persistent (or any conversation when the unit is deleted)
 * is finished.
 *
 * Definition fields resolve to the DB ai_conversation struct (scenario ai_conversations, 116-byte
 * stride); actor fields use named actor_datum members. */

#include <stdint.h>
#include "headers/ai_conversation.h"
#include "headers/ai_conversation_definition_flags.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/conversation_datum.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);

void ai_conversation_unit_died(int unit_index, uint8_t deleted)
{
    data_iterator iterator;
    data_iterator_new(&iterator, conversation_data);

    conversation_datum *conversation = data_iterator_next(&iterator);
    if ( !conversation )
        return;

    while ( 1 )
    {
        char referenced = 0;
        ai_conversation *definition = (ai_conversation *)global_scenario->ai_conversations.address
                           + conversation->conversation_definition_index;

        if ( conversation->line_unit_index == unit_index )
        {
            referenced = 1;
            conversation->line_advance = 1;
            conversation->line_unit_index = -1;
        }
        if ( conversation->line_address_unit_index == unit_index )
        {
            referenced = 1;
            conversation->line_address_unit_index = -1;
        }
        if ( conversation->triggering_player_unit_index == unit_index )
        {
            referenced = 1;
            conversation->triggering_player_unit_index = -1;
        }

        if ( deleted || (definition->flags & (1u << _ai_conversation_stop_if_anyone_dies_bit)) != 0 )
        {
            int participant_count = definition->participants.count;
            for ( int participant = 0; participant < participant_count; participant = (int16_t)(participant + 1) )
            {
                if ( ((1 << participant) & conversation->participant_bitmask) != 0
                  && conversation->actor_indices[participant] != -1 )
                {
                    actor_datum *actor = DATUM_GET(actor_data, actor_datum,
                                  conversation->actor_indices[participant]);
                    if ( actor->meta.unit_index == unit_index )
                        referenced = 1;
                    if ( deleted )
                    {
                        if ( actor->state.action == actor_action_converse
                          && actor->state.action_data.___u0.converse.run_to_unit_index == unit_index )
                            actor->state.action_data.___u0.converse.run_to_unit_index = -1;
                        if ( actor->external_orders.conversation_attention_unit_index == unit_index )
                            actor->external_orders.conversation_attention_unit_index = -1;
                    }
                }
            }
            if ( referenced )
                break;
        }

        conversation = data_iterator_next(&iterator);
        if ( !conversation )
            return;
    }

    ai_conversation_finish(iterator.index, 0, 0);
}
