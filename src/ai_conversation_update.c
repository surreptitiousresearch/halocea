/* ai_conversation_update @ 0x837CFDB8 — drive every live AI conversation one tick. For each runtime
 * conversation datum: if not yet begun, try to begin it once a second (creation_time-relative) and finish
 * it as "unable to begin" if begin gave up; if begun and not finishing, advance through its lines
 * (perform/begin each, bumping line_index until the definition's line count is reached, then mark
 * finishing); finish a finishing conversation; otherwise, for a begun conversation, refresh each
 * participant actor's assigned conversation and addressee unit from the line's speaker/listener pairing.
 *
 * The scenario conversation definition (ai_conversation, 116 bytes) is reached through
 * global_scenario->ai_conversations.address; participants.count @def+0x50, lines.count @def+0x5C.
 * Participant actors carry their current conversation index and addressee unit in
 * actor_datum.external_orders (conversation_index / conversation_attention_unit_index). */

#include <stdint.h>
#include "headers/ai_conversation.h"
#include "headers/conversation_line_flags.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/conversation_datum.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

extern uint8_t ai_conversation_begin(uint16_t conversation_index, uint8_t *continue_trying);
extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);
extern uint8_t ai_conversation_line_perform(int conversation_index);
extern int ai_conversation_line_begin(uint16_t conversation_index);

void ai_conversation_update(void)
{
    data_iterator       iterator;
    conversation_datum *conversation;
    int                 time = game_time_get();

    data_iterator_new(&iterator, conversation_data);
    for ( conversation = data_iterator_next(&iterator);
          conversation;
          conversation = data_iterator_next(&iterator) )
    {
        ai_conversation *definition = (ai_conversation *)global_scenario->ai_conversations.address
                         + conversation->conversation_definition_index;

        if ( !conversation->begun )
        {
            unsigned __int8 continue_trying = 1;
            if ( (time - conversation->creation_time) % 30 == 0 )
                ai_conversation_begin(iterator.index, &continue_trying);
            if ( !conversation->begun && !continue_trying )
                ai_conversation_finish(iterator.index, 1u, 0);
        }

        if ( conversation->begun && !conversation->finished )
        {
            unsigned __int16 line = conversation->line_index;
            unsigned __int8 advance = (line < 0x8000 && (__int16)line < definition->lines.count) ? 1 : 0;

            for ( unsigned __int8 perform = advance;
                  !perform || ai_conversation_line_perform(iterator.index);
                  perform = ai_conversation_line_begin(iterator.index) )
            {
                __int16 next_line = (__int16)(conversation->line_index + 1);
                conversation->line_index = next_line;
                if ( next_line >= definition->lines.count )
                {
                    conversation->finished = 1;
                    break;
                }
            }
        }

        if ( conversation->finished )
        {
            ai_conversation_finish(iterator.index, 0, 1u);
        }
        else if ( conversation->begun && definition->participants.count > 0 )
        {
            for ( __int16 participant = 0; participant < definition->participants.count; participant = (__int16)(participant + 1) )
            {
                if ( ((1 << participant) & conversation->participant_bitmask) != 0
                     && conversation->actor_indices[participant] != -1 )
                {
                    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, conversation->actor_indices[participant]);
                    int actor_unit = actor->meta.unit_index;

                    actor->external_orders.conversation_index = iterator.index;              /* current conversation */
                    actor->external_orders.conversation_attention_unit_index = -1;            /* addressee unit (default none) */

                    int speaker_unit = conversation->line_unit_index;
                    if ( actor_unit == speaker_unit )
                    {
                        actor->external_orders.conversation_attention_unit_index = conversation->line_address_unit_index;
                    }
                    else
                    {
                        int address_unit = conversation->line_address_unit_index;
                        if ( actor_unit == address_unit && (conversation->line_flags & (1u << _ai_conversation_line_addressee_look_back_bit)) != 0 )
                        {
                            actor->external_orders.conversation_attention_unit_index = speaker_unit;
                        }
                        else if ( (conversation->line_flags & (1u << _ai_conversation_line_everyone_look_at_speaker_bit)) != 0 )
                        {
                            actor->external_orders.conversation_attention_unit_index = speaker_unit;
                        }
                        else if ( (conversation->line_flags & (1u << _ai_conversation_line_everyone_look_at_addressee_bit)) != 0 )
                        {
                            actor->external_orders.conversation_attention_unit_index = address_unit;
                        }
                    }
                }
            }
        }
    }
}
