/* ai_conversation_advance @0x837CCF00 — request that every running runtime conversation matching the given
 * definition index advance to its next line, by setting the advance flag (conversation+9). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/conversation_datum.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

void ai_conversation_advance(int16_t conversation_definition_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, conversation_data);
    for ( conversation_datum *conversation = data_iterator_next(&iterator);
          conversation;
          conversation = data_iterator_next(&iterator) )
    {
        if ( conversation->conversation_definition_index == conversation_definition_index )
            conversation->told_to_advance = 1;
    }
}
