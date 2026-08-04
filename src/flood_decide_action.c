/* flood_decide_action @0x8381F038 — per-tick action-class dispatch for the "flood" species' actor AI
 * (sibling of infection_decide_action.c / hunter_decide_action.c). Runs the common housekeeping
 * (initial-action, pending-command-list, surprise, and — unless a transition is denied — berserk-from-damage,
 * berserk-transition, combat-transition, danger-avoidance), then dispatches on the current action class:
 *   3/0xA (combat/failure): fall back to combat-failure then evasion if combat-status declines;
 *   4 (flee): finish fleeing, unless still "wants to flee" (+170), in which case force a combat-status decision;
 *   5/7/8 (pursuit, shared): fall back to exit-pursuit if combat-status declines;
 *   6 (guard): combat-status using actor_action_can_stop_guarding as its allow_initiative;
 *   0xB: combat-status using two actor-state flags (+158 allow_initiative, +161 force_decision) directly;
 *   0xD: no-op if +640 is set, otherwise force a combat-status decision;
 *   everything else: no-op.
 *
 * DEVIATION: the action_data bytes are resolved to the live union arm selected by each switch case
 * (flee.unable_to_flee, obey.initiative/finished). +108 reuses actor_state_data.action. The decompiler's
 * LABEL_15/LABEL_16 combat-status merge points were expanded into direct calls per branch
 * (allow_initiative, force_decision). +640 is resolved as actor->danger_zone.danger_type. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/data_array.h"
#include "headers/actor_action.h"
#include "headers/actor_berserk_type.h"
#include "headers/actor_surprise_type.h"
#include "headers/blam_data_globals.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level);
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_damage(uint16_t actor_index);
extern int actor_action_handle_berserk_transition(int actor_index, int16_t berserk_level);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern unsigned __int8 actor_action_can_stop_guarding(int actor_index, __int16 guard_investigate_threshold,
    __int16 cower_investigate_threshold);

void flood_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    actor_action_handle_surprise(actor_index, _actor_surprise_unprepared_grenade);
    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_berserking_from_damage(actor_index);
        actor_action_handle_berserk_transition(actor_index, actor_berserk_damage);
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_danger_avoidance(actor_index);
    }

    switch ( actor->state.action )
    {
        case actor_action_fight:
        case actor_action_charge:
        {
            unsigned __int8 allow_initiative = 1;
            unsigned __int8 force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision)
              && !actor_action_handle_combat_failure(actor_index) )
                actor_action_handle_evasion(actor_index);
            return;
        }

        case actor_action_flee:
            if ( !actor->state.action_data.___u0.flee.unable_to_flee )
            {
                actor_action_handle_done_fleeing(actor_index);
                return;
            }
            {
                unsigned __int8 allow_initiative = 1;
                unsigned __int8 force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            return;

        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
        {
            unsigned __int8 allow_initiative = 1;
            unsigned __int8 force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_exit_pursuit(actor_index);
            return;
        }

        case actor_action_guard:
        {
            unsigned __int8 allow_initiative = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
            unsigned __int8 force_decision = 0;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            return;
        }

        case actor_action_obey:
            actor_action_handle_combat_status(actor_index, actor->state.action_data.___u0.obey.initiative, actor->state.action_data.___u0.obey.finished);
            return;

        case actor_action_avoid:
            if ( actor->danger_zone.danger_type )
                return;
            {
                unsigned __int8 allow_initiative = 1;
                unsigned __int8 force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            return;

        default:
            return;
    }
}
