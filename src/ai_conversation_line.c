/* ai_conversation_line @0x837CCE78 — return the current line index of the active runtime conversation whose
 * definition index matches, or 999 if no such conversation is running. Walks the conversation datum array.
 * Conversation datum: definition index at +2 (word 1), current line index at +72 (word 36). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/conversation_datum.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int ai_conversation_line(int16_t conversation_definition_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, conversation_data);
    for ( conversation_datum *conversation = data_iterator_next(&iterator);
          conversation;
          conversation = data_iterator_next(&iterator) )
    {
        if ( conversation->conversation_definition_index == conversation_definition_index )
            return (uint16_t)conversation->line_index;
    }
    return 999;
}
