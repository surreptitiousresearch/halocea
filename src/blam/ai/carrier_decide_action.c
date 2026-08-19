/* carrier_decide_action @0x838272D8 — per-tick action-class dispatch for the "carrier"/hunter-pair-style
 * actor AI. Runs the common initial/pending-command-list housekeeping plus a surprise check, then (unless
 * a transition is denied) runs the full berserk pipeline (attacking-mode/damage/proximity triggers, then
 * berserk_transition, then the usual combat_transition) before dispatching per action class (3-11): flee
 * (4) — finish fleeing unless still "wants to flee" (+170), otherwise force combat-status; guard (6) — may
 * stop guarding based on actor_action_can_stop_guarding; pursuit-related (5/7/8, one path) — fall back to
 * exit-pursuit if combat-status declines; failure-related (0xA/3) — fall back to combat-failure, and if
 * that ALSO declines, force an evasion (the one behavioral difference from the sibling
 * infection_decide_action.c, which has no evasion fallback here); idle (9) — no-op; the numeric default
 * defers to combat-status using two obey-state flags (obey_state_data.initiative/.finished) directly as its
 * allow_initiative/force_decision arguments.
 *
 * The action_class values are actor_action enum members (state.action); the per-class scratch fields are
 * read through the matching arm of the state.action_data tagged union — .flee.unable_to_flee for the flee
 * class and .obey.initiative/.finished for the obey default (the union arm that is live is selected by
 * state.action). The guarding thresholds and berserk level passed below are
 * an actor_combat_status level and an actor_berserk_type value respectively (the callees compare against
 * state.combat_status / stimuli.berserk_type). The surprise level (4) is a raw stimuli.surprise_level
 * threshold with no enum. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/actor_action.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_berserk_type.h"
#include "headers/actor_surprise_type.h"
#include "headers/blam_data_globals.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level);
extern uint8_t actor_action_deny_transition(int actor_index);
extern uint8_t actor_action_handle_berserking_from_attacking_mode(int actor_index);
extern uint8_t actor_action_handle_berserking_from_damage(int actor_index);
extern uint8_t actor_action_handle_berserking_from_proximity(int actor_index);
extern int actor_action_handle_berserk_transition(int actor_index, int16_t berserk_level);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_can_stop_guarding(int actor_index, int16_t guard_investigate_threshold, int16_t cower_investigate_threshold);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);

void carrier_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    actor_action_handle_surprise(actor_index, _actor_surprise_unprepared_grenade);

    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_berserking_from_attacking_mode(actor_index);
        actor_action_handle_berserking_from_damage(actor_index);
        actor_action_handle_berserking_from_proximity(actor_index);
        actor_action_handle_berserk_transition(actor_index, actor_berserk_attacking);
        actor_action_handle_combat_transition(actor_index);
    }

    int16_t action_class = actor->state.action;
    if ( (unsigned int)(action_class - actor_action_fight) > 8 )   /* only classes fight(3)..obey(11) */
        return;

    uint8_t allow_initiative;
    uint8_t force_decision;

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
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) && !actor_action_handle_combat_failure(actor_index) )
                actor_action_handle_evasion(actor_index);
            return;

        default:  /* actor_action_obey (11) */
            break;
    }

    force_decision = actor->state.action_data.___u0.obey.finished;
    allow_initiative = actor->state.action_data.___u0.obey.initiative;
    actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
}
