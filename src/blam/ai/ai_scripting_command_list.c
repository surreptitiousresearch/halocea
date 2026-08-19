/* ai_scripting_command_list @0x83771658 — script command: order every actor selected by the AI index to obey a
 * scenario command list. For each actor, set up the obey state and, if the actor will obey, switch it into the
 * command-list action (actor_action_obey). */

#include <stdint.h>
#include "headers/ai_index_actor_iterator.h"
#include "headers/obey_state_data.h"
#include "headers/action_state_data.h"
#include "headers/actor_action.h"

#include "headers/actor_datum.h"
#include "headers/obey_state_data.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern uint8_t action_obey_command_list_setup(int actor_index, int16_t command_list_index, obey_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

void ai_scripting_command_list(int ai_index, int16_t command_list_index)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
    {
        action_state_data action_data;
        if ( action_obey_command_list_setup(iterator.iterator.index, command_list_index,
                                            &action_data.___u0.obey) )
            actor_action_change(iterator.iterator.index, actor_action_obey, &action_data);
    }
}
