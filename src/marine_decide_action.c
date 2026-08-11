/* marine_decide_action @0x83828DC0 — the per-tick action-selection brain for a "marine"-class actor. First it
 * runs the unconditional pre-transition handlers (initial action, pending command list, surprise), then — unless
 * a transition is currently denied — the panic/berserk/combat/vehicle/grenade/danger transition handlers, using
 * a lower berserk level (3 vs 5) when the actor's berserk-eagerness byte (+522) is small. Finally it dispatches
 * on the actor's current action (state.action) to run the follow-up handler appropriate to that action: combat
 * status / failure / evasion for guarding & searching, done-fleeing for fleeing, exit-pursuit for pursuit, and a
 * combat-status re-evaluation (with per-action allow-initiative / force-decision flags read from the action's
 * working state) for the guard/cower/uncover/conversation/retreat actions.
 *
 * The actor is typed as actor_datum; state.action drives the dispatch. The action_data bytes are resolved
 * to the live union arm selected by each switch case (flee/vehicle/obey/converse). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_action.h"
#include "headers/actor_type.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_berserk_type.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_surprise_type.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level);
extern uint8_t actor_action_deny_transition(int actor_index);
extern uint8_t actor_action_handle_panic_from_damage(int actor_index);
extern uint8_t actor_action_handle_panic_from_attached_projectiles(int actor_index);
extern int actor_action_handle_panic_from_attached_melee_attackers(int actor_index);
extern uint8_t actor_action_handle_panic_from_burning_to_death(int actor_index);
extern int actor_action_handle_panic_transition(int actor_index, int16_t minimum_panic_level, uint8_t enforced_calm, int16_t force_panic_level);
extern uint8_t actor_action_handle_berserking_from_damage(int actor_index);
extern int actor_action_handle_berserk_transition(int actor_index, int16_t berserk_level);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern uint8_t actor_action_handle_vehicle_entry(int actor_index);
extern int actor_action_handle_vehicle_exit(int actor_index);
extern uint8_t actor_action_handle_grenade_throwing(int actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern uint8_t actor_action_can_stop_guarding(int actor_index, int16_t guard_investigate_threshold, int16_t cower_investigate_threshold);
extern uint8_t actor_action_can_stop_conversing(int actor_index);

void marine_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    actor_action_handle_surprise(actor_index, _actor_surprise_unprepared_enemy_shooting);

    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_panic_from_damage(actor_index);
        actor_action_handle_panic_from_attached_projectiles(actor_index);
        actor_action_handle_panic_from_attached_melee_attackers(actor_index);
        actor_action_handle_panic_from_burning_to_death(actor_index);
        actor_action_handle_panic_transition(actor_index, _actor_panic_damage, 0, _actor_panic_never);
        actor_action_handle_berserking_from_damage(actor_index);
        /* few marine friends nearby -> berserk from damage-level stimuli; otherwise effectively never */
        int16_t berserk_level = actor->situation.area_friends_by_type[_actor_type_marine] <= 2
                ? actor_berserk_damage : actor_berserk_never;
        actor_action_handle_berserk_transition(actor_index, berserk_level);
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_vehicle_entry(actor_index);
        actor_action_handle_vehicle_exit(actor_index);
        actor_action_handle_grenade_throwing(actor_index);
        actor_action_handle_danger_avoidance(actor_index);
    }

    switch ( actor->state.action )
    {
        case actor_action_fight:
        case actor_action_charge:
        {
            uint8_t allow_initiative = 1;
            uint8_t force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision)
              && !actor_action_handle_combat_failure(actor_index) )
                actor_action_handle_evasion(actor_index);
            break;
        }
        case actor_action_flee:
            if ( actor->state.action_data.___u0.flee.unable_to_flee )
            {
                uint8_t allow_initiative = 1;
                uint8_t force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            else
                actor_action_handle_done_fleeing(actor_index);
            break;
        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
        {
            uint8_t allow_initiative = 1;
            uint8_t force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_exit_pursuit(actor_index);
            break;
        }
        case actor_action_guard:
        {
            uint8_t allow_initiative = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
            uint8_t force_decision = 0;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            break;
        }
        case actor_action_vehicle:
            if ( actor->state.action_data.___u0.vehicle.vehicle_entry_done || actor->state.action_data.___u0.vehicle.vehicle_entry_failed )
            {
                uint8_t allow_initiative = 1;
                uint8_t force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            break;
        case actor_action_obey:
            actor_action_handle_combat_status(actor_index, actor->state.action_data.___u0.obey.initiative, actor->state.action_data.___u0.obey.finished);
            break;
        case actor_action_converse:
        {
            uint8_t force_decision = (actor->state.action_data.___u0.converse.failed || actor->external_orders.conversation_index == -1) ? 1 : 0;
            uint8_t allow_initiative = actor_action_can_stop_conversing(actor_index);
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            break;
        }
        case actor_action_avoid:
            if ( !actor->danger_zone.danger_type )
            {
                uint8_t allow_initiative = 1;
                uint8_t force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            break;
        default:
            break;
    }
}
