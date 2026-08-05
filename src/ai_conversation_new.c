/* ai_conversation_new @0x837CD100 — allocate a runtime conversation for a definition index and return its datum
 * index (or -1 on failure). If the conversation pool is full and this request is scripted, the lowest-priority /
 * oldest existing conversation is evicted (the one with the smallest "scripted" rank, breaking ties by earliest
 * creation time), finished, and its slot reused. Non-scripted requests do not evict.
 *
 * Conversation datum is 100 bytes: definition index at +2, scripted rank at +4, creation time at +12,
 * line_index at +72 (initialized to -1). */

#include <stdint.h>
#include "headers/conversation_datum.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern int datum_new_at_index(data_array *data, int index);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);
extern int game_time_get(void);

int ai_conversation_new(int16_t conversation_definition_index, char scripted)
{
    int conversation_index = datum_new(conversation_data);

    if ( conversation_index == -1 )
    {
        if ( !scripted )
            return -1;

        unsigned char lowest_rank = 1;
        int oldest_time = 0x7FFFFFFF;
        int evict_index = -1;

        data_iterator iterator;
        data_iterator_new(&iterator, conversation_data);
        for ( unsigned char *conversation = data_iterator_next(&iterator);
              conversation;
              conversation = data_iterator_next(&iterator) )
        {
            conversation_datum *conv = (conversation_datum *)conversation;
            if ( conv->scripted < (unsigned int)lowest_rank || conv->creation_time < oldest_time )
            {
                oldest_time = conv->creation_time;
                lowest_rank = conv->scripted;
                evict_index = iterator.index;
            }
        }

        if ( evict_index == -1 )
            return -1;

        ai_conversation_finish(evict_index, 0, 0);
        conversation_index = datum_new_at_index(conversation_data, evict_index);
        if ( conversation_index == -1 )
            return -1;
    }

    conversation_datum *conversation = DATUM_GET(conversation_data, conversation_datum, conversation_index);
    conversation->conversation_definition_index = conversation_definition_index;
    conversation->line_index = -1;  /* disasm: sth -1, 0x48 — line_index (+72), not line_participant_index (+74) */
    conversation->scripted = scripted;
    conversation->creation_time = game_time_get();
    return conversation_index;
}
