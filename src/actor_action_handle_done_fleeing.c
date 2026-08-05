/* actor_action_handle_done_fleeing @0x837F6488 — if the actor's current action is fleeing (word +108 == 4)
 * and its flee state (action data block at +156, reinterpreted as flee_state_data) recorded
 * done_fleeing (+171 = flee_state_data.done_fleeing at 0x0F within the 132-byte action_state_data block —
 * offset 156+15=171), convert it into a guard state and switch the actor to guarding (action type 6). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/guard_state_data.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


#include "headers/flee_state_data.h"
extern int action_guard_setup_from_fleeing(uint16_t actor_index, flee_state_data *flee_state_data, guard_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t actor_action_handle_done_fleeing(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action == actor_action_flee && actor->state.action_data.___u0.flee.done_fleeing )
    {
        guard_state_data guard_state;

        action_guard_setup_from_fleeing((uint16_t)actor_index, &actor->state.action_data.___u0.flee, &guard_state);
        actor_action_change(actor_index, actor_action_guard, (action_state_data *)&guard_state);
        return 1;
    }

    return 0;
}
