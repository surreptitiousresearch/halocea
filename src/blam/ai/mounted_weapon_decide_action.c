/* mounted_weapon_decide_action @0x83826FF8 — per-tick action-mode dispatch for an actor on a mounted
 * weapon: runs the initial/pending-command handling and (unless a transition is denied) the combat
 * transition, then dispatches on the actor's action mode (word +108, values 3..11 handled): modes 3/4/10
 * request combat status and fall back to actor_action_handle_combat_failure; modes 5/6/7/8 request combat
 * status and fall back to actor_action_handle_exit_pursuit; mode 9 is a no-op; mode 11 requests combat
 * status using the actor's own raw allow-initiative/force-decision bytes (+158/+161) instead of the usual
 * fixed (1,0). Modes outside [3,11] are left alone entirely. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern uint8_t actor_action_deny_transition(int actor_index);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern uint8_t actor_action_handle_combat_failure(int actor_index);

void mounted_weapon_decide_action(int actor_index)
{
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    if ( !actor_action_deny_transition(actor_index) )
        actor_action_handle_combat_transition(actor_index);

    int16_t action_mode = actor->state.action;
    if ( (unsigned int)(action_mode - actor_action_fight) > 8 )   /* only fight(3)..obey(11) */
        return;

    switch ( action_mode )
    {
    case actor_action_fight:
    case actor_action_flee:
    case actor_action_charge:
        if ( !actor_action_handle_combat_status(actor_index, 1u, 0) )
            actor_action_handle_combat_failure(actor_index);
        break;

    case actor_action_uncover:
    case actor_action_guard:
    case actor_action_search:
    case actor_action_wait:
        if ( !actor_action_handle_combat_status(actor_index, 1u, 0) )
            actor_action_handle_exit_pursuit(actor_index);
        break;

    case actor_action_vehicle:
        break;

    default:   /* 11 */
        actor_action_handle_combat_status(actor_index,
                actor->state.action_data.___u0.obey.initiative,
                actor->state.action_data.___u0.obey.finished);
        break;
    }
}
