/* ai_scripting_command_list_advance @0x83771760 — script command: advance every actor selected by the AI index
 * to the next command in its current command list. */

#include "headers/ai_index_actor_iterator.h"

#include "headers/actor_datum.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void action_obey_advance_command_list(int actor_index);

void ai_scripting_command_list_advance(int ai_index)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
        action_obey_advance_command_list(iterator.iterator.index);
}
