/* ai_conversation_status @0x837CCCF8 — report the status of the conversation with the given definition index.
 * First scans the running conversation data for a match and returns the highest live status:
 *   1 = created but not yet begun        2 = begun, not paused
 *   3 = begun and paused (paused flag at conversation+8 set => 4? see below)
 * (Live status: conversation+6 begun flag; if begun, conversation+5 paused-capable => 3 + (paused bit at +8),
 *  else 2; if not begun => 1.) When no conversation is live (status 0), it consults ai_globals.recent_conversations
 * for the most recently finished instance of this definition and returns 5 (unable to begin), 6 (finished
 * successfully) or 7 (finished unsuccessfully). Returns 0 when nothing is known.
 *
 * Conversation datum: definition index at +2 (word 1), begun flag at +6, paused-capable flag at +5, paused bit
 * source at +8. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/conversation_datum.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int ai_conversation_status(int16_t conversation_definition_index)
{
    int status = 0;

    data_iterator iterator;
    data_iterator_new(&iterator, conversation_data);
    for ( conversation_datum *conversation = data_iterator_next(&iterator);
          conversation;
          conversation = data_iterator_next(&iterator) )
    {
        if ( conversation->conversation_definition_index != conversation_definition_index )
            continue;

        __int16 conversation_status;
        if ( conversation->begun )
        {
            if ( conversation->any_line_spoken )
                conversation_status = (conversation->waiting_to_advance != 0) + 3;   /* paused bit at +8 */
            else
                conversation_status = 2;
        }
        else
        {
            conversation_status = 1;
        }
        if ( (__int16)status > conversation_status )
            conversation_status = status;
        status = conversation_status;
    }

    if ( (__int16)status )
        return status;

    /* No live conversation — look up the most recent finished instance. */
    int latest_finish_time = -1;
    __int16 latest_index = -1;
    int count = ai_globals->recent_conversation_count;
    for ( int i = 0; i < count; i = (__int16)(i + 1) )
    {
        if ( ai_globals->recent_conversations[i].definition_index == conversation_definition_index
          && ai_globals->recent_conversations[i].finish_time > latest_finish_time )
        {
            latest_index = i;
            latest_finish_time = ai_globals->recent_conversations[i].finish_time;
        }
    }

    if ( latest_index == -1 )
        return status;

    recent_conversation *recent = &ai_globals->recent_conversations[latest_index];
    if ( recent->unable_to_begin )
        return 5;
    return (recent->finished_successfully == 0) + 6;
}
