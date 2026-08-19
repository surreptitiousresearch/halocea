/* actor_decision_loop @0x8371F750 — per-tick AI decision cycle: repeatedly decide+clear stimuli
 * (actor_type_decide_action / actor_stimulus_clear), stopping once the action-changed flag (+112, cleared
 * each pass) settles or after 10 passes, then performs the chosen action. If the action neither changed nor
 * needs performing on the very first pass, bails out early without touching the current action. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

#include "headers/action_state_data.h"
extern void actor_type_decide_action(int actor_index);
extern void actor_stimulus_clear(int actor_index);
extern uint8_t actor_action_perform(int actor_index);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

void actor_decision_loop(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int pass_count = 0;
    int action_performed = 0;

    while ( 1 )
    {
        actor->state.action_changed = 0;
        ++pass_count;

        actor_type_decide_action(actor_index);
        actor_stimulus_clear(actor_index);

        uint8_t done = (action_performed && !actor->state.action_changed) || pass_count >= 10;
        if ( done )
            break;

        action_performed = actor_action_perform(actor_index);
        if ( !action_performed && !actor->state.action_changed )
            return;
    }

    actor_action_change(actor_index, actor_action_none, 0);
}
