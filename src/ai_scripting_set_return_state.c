/* ai_scripting_set_return_state @0x83771D80 — set the AI default ("return") state each actor under an AI
 * index falls back to. Stores the new default on every actor; for actors that are not currently in combat
 * and whose active action class is idle/guard-like (class 0, 1, or 2), it also clears the current default
 * state (-1) so they re-evaluate into the new return state. Ignored if the state value is out of range. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/actor_default_state.h"
#include "headers/action_class.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern int16_t actor_action_class(int actor_index);
extern uint8_t actor_action_set_default_state(int actor_index, int16_t override_state);

void ai_scripting_set_return_state(int ai_index, int16_t default_state)
{
    if ( (unsigned int)default_state < number_of_actor_default_states )
    {
        ai_index_actor_iterator iterator;
        ai_index_actor_iterator_new(ai_index, &iterator);
        for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
              actor;
              actor = ai_index_actor_iterator_next(&iterator) )
        {
            int16_t action_class = actor_action_class(iterator.iterator.index);
            int combat_status = (uint16_t)actor->state.combat_status;
            actor->state.default_state = default_state;
            if ( !combat_status && (action_class == _action_class_noncombat
                                    || action_class == _action_class_passive
                                    || action_class == _action_class_transitory) )
                actor_action_set_default_state(iterator.iterator.index, -1);
        }
    }
}
