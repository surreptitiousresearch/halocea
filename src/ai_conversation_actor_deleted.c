/* ai_conversation_actor_deleted @0x837CE8A8 — when an actor is deleted, walk every live AI conversation and
 * remove it from each participant list. For a conversation that can survive losing this participant
 * (definition +0x20 bit 0x01 clear) the actor is just dropped: its participant bit is cleared, its
 * actor_indices slot is set to -1, and if it was the current speaker (line_participant_index) the conversation
 * is told to advance (line_advance = 1). For a conversation that cannot continue without it, the whole
 * conversation is finished (ai_conversation_finish), which the compiler inlined here.
 *
 * Deviation: the finish path is the inlined body of ai_conversation_finish(index, 0, 0); reconstructed as a
 * call to that function. The conversation definition (116-byte stride) is resolved to the DB ai_conversation struct. */

#include <stdint.h>
#include "headers/ai_conversation.h"
#include "headers/ai_conversation_definition_flags.h"
#include "headers/data_array.h"
#include "headers/conversation_datum.h"
#include "headers/scenario.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);

void ai_conversation_actor_deleted(int actor_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, conversation_data);
    for ( conversation_datum *conversation = data_iterator_next(&iterator);
          conversation;
          conversation = data_iterator_next(&iterator) )
    {
        ai_conversation *definition = (ai_conversation *)global_scenario->ai_conversations.address
                           + conversation->conversation_definition_index;
        int participant_count = definition->participants.count;
        if ( participant_count <= 0 )
            continue;

        for ( int participant = 0; participant < participant_count; participant = (__int16)(participant + 1) )
        {
            if ( conversation->actor_indices[participant] != actor_index )
                continue;

            if ( (definition->flags & (1u << _ai_conversation_stop_if_anyone_dies_bit)) == 0 )
            {
                conversation->participant_bitmask &= ~(1u << participant);
                conversation->actor_indices[participant] = -1;
                if ( conversation->line_participant_index == participant )
                    conversation->line_advance = 1;
                continue;
            }

            /* This participant is required: finish the whole conversation. */
            if ( iterator.index != -1 )
                ai_conversation_finish(iterator.index, 0, 0);
            break;
        }
    }
}
