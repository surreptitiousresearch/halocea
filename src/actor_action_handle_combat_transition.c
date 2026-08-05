/* actor_action_handle_combat_transition @0x837F6948 — while an actor hasn't yet entered a late-stage action
 * state (state.mode < _actor_mode_combat) and has a pending combat-transition request flagged (stimuli.combat_transition),
 * consumes that flag and enters the guard action (falling back to plain combat selection if the guard setup
 * can't produce a valid state). Returns 1 if the transition was handled, 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/action_state_data.h"
#include "headers/actor_datum.h"
#include "headers/actor_mode.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


#include "headers/guard_state_data.h"
extern uint8_t action_guard_setup_from_combat_transition(int actor_index, guard_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);
extern uint8_t actor_action_handle_combat_selection(int actor_index);

uint8_t actor_action_handle_combat_transition(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.mode >= _actor_mode_combat || !actor->stimuli.combat_transition )
        return 0;

    actor->state.mode = _actor_mode_combat;

    action_state_data new_action_data;
    if ( action_guard_setup_from_combat_transition(actor_index, &new_action_data.___u0.guard) )
        actor_action_change(actor_index, actor_action_guard, &new_action_data);
    else
        actor_action_handle_combat_selection(actor_index);

    actor->stimuli.combat_transition = 0;
    return 1;
}
