/* infection_decide_action @0x83827468 — per-tick action-class dispatch for the "infection" gametype's
 * actor AI: runs the common initial/pending-command-list/combat-transition housekeeping, then (only for
 * action classes 3-11) dispatches per class — flee (4): finish fleeing unless still "wants to flee"
 * (+170), otherwise hand off to combat-status with force_decision; guard (6): may stop guarding based on
 * actor_action_can_stop_guarding; pursuit-related (5/7/8, sharing one path): fall back to exit-pursuit if
 * combat-status declines; failure-related (0xA/3): fall back to combat-failure if combat-status declines;
 * idle (9): no-op; the numeric default (everything else in range): defers to combat-status using the two
 * obey-state flags (obey.initiative/.finished) directly as its allow_initiative/force_decision arguments.
 *
 * The per-class scratch fields are read through the matching arm of the state.action_data tagged union
 * (.flee.unable_to_flee for flee, .obey.initiative/.finished for the obey default). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/data_array.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern unsigned __int8 actor_action_can_stop_guarding(int actor_index, __int16 guard_investigate_threshold,
    __int16 cower_investigate_threshold);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern uint8_t actor_action_handle_combat_failure(int actor_index);

void infection_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    if ( !actor_action_deny_transition(actor_index) )
        actor_action_handle_combat_transition(actor_index);

    __int16 action_class = actor->state.action;
    if ( (unsigned int)(action_class - actor_action_fight) > 8 )   /* only fight(3)..obey(11) */
        return;

    unsigned __int8 allow_initiative;
    unsigned __int8 force_decision;

    switch ( action_class )
    {
        case actor_action_flee:
            if ( !actor->state.action_data.___u0.flee.unable_to_flee )
            {
                actor_action_handle_done_fleeing(actor_index);
                return;
            }
            force_decision = 1;
            allow_initiative = 1;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            return;

        case actor_action_guard:
            allow_initiative = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
            force_decision = 0;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            return;

        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
            allow_initiative = 1;
            force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_exit_pursuit(actor_index);
            return;

        case actor_action_vehicle:
            return;

        case actor_action_charge:
        case actor_action_fight:
            allow_initiative = 1;
            force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_combat_failure(actor_index);
            return;

        default:
            break;
    }

    force_decision = actor->state.action_data.___u0.obey.finished;
    allow_initiative = actor->state.action_data.___u0.obey.initiative;
    actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
}
