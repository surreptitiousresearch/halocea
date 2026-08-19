/* ai_scripting_set_current_state @0x83771E38 — force every actor under an AI index into the given AI
 * default state (0..11). Ignored if the state value is out of range. */

#include <stdint.h>
#include "headers/ai_index_actor_iterator.h"
#include "headers/actor_default_state.h"

#include "headers/actor_datum.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern uint8_t actor_action_set_default_state(int actor_index, int16_t override_state);

void ai_scripting_set_current_state(int ai_index, int16_t current_state)
{
    if ( (unsigned int)current_state < number_of_actor_default_states )
    {
        ai_index_actor_iterator iterator;
        ai_index_actor_iterator_new(ai_index, &iterator);
        while ( ai_index_actor_iterator_next(&iterator) )
            actor_action_set_default_state(iterator.iterator.index, current_state);
    }
}
