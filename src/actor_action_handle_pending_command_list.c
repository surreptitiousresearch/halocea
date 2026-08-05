/* actor_action_handle_pending_command_list @0x837F42F8 — if the actor has a pending scripted command list
 * (word +144), enter the obey action (11) for it when the pending-forced flag byte (+142) is set, or —
 * for a non-forced request — when the actor is in a nonzero awareness state (word +106) and the current
 * action does not refuse to yield (actor_action_deny_transition). The pending list and flag are consumed
 * whenever the request isn't blocked. Returns 1 when the obey action was entered. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/obey_state_data.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


#include "headers/action_state_data.h"
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t action_obey_command_list_setup(int actor_index, int16_t command_list_index, obey_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t actor_action_handle_pending_command_list(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uint8_t entered_obey = 0;

    if ( actor->state.command_list_index == 0xFFFF )
        return 0;

    uint8_t blocked = 0;
    if ( actor->state.command_list_immediate )
        blocked = 0;
    else if ( !actor->state.mode || actor_action_deny_transition(actor_index) )
        blocked = 1;

    if ( !blocked )
    {
        obey_state_data obey_state;
        if ( action_obey_command_list_setup(actor_index, actor->state.command_list_index, &obey_state) )
        {
            /* obey_state_data is the actor_action_obey arm of the action_state_data union */
            actor_action_change(actor_index, actor_action_obey, (action_state_data *)&obey_state);
            entered_obey = 1;
        }
        actor->state.command_list_index = -1;
        actor->state.command_list_immediate = 0;
    }
    return entered_obey;
}
