/* grunt_decide_action @0x838293F8 — per-tick action-class dispatch for the "grunt" actor AI (cowardly covenant
 * infantry emboldened by a nearby leader). Runs the common initial/pending-command-list housekeeping plus a
 * surprise check, then (unless a transition is denied) runs the full panic pipeline — with calm enforced while a
 * leader-type ally is nearby (+515 counter > 0) and a force-panic level of 7 — followed by combat transition,
 * vehicle entry/exit, grenade throwing and danger avoidance, before dispatching on the action class (state.action):
 *   3/0xA : force combat-status, else combat-failure, else evasion.
 *   4     : flee — if a leader-type ally was very close (+583 counter > 0) and the flee panic_type (+168) is set
 *           but no blind panic results, latch +171; if still "wants to flee" (+170) force combat-status(1,1);
 *           otherwise finish fleeing, and when not panicking with combat_status >= _actor_combat_status_clear_los consider a retreat grenade.
 *   5/7/8 : combat-status, else exit-pursuit.
 *   6     : guard — defer to actor_action_can_stop_guarding(definite, dangerous) for combat-status.
 *   9     : if a guard-state byte (+165/+166) is set, force combat-status(1,1).
 *   0xB   : combat-status using two actor-state flags (+158, +161) directly.
 *   0xC   : converse — combat-status via actor_action_can_stop_conversing, forcing decision when done
 *           conversing (+160) or with no conversation (+476 == -1).
 *   0xD   : if no scripted-freeze word (+640) force combat-status(1,1).
 *   default: no-op.
 *
 * Same template as the sibling *_decide_action files (jackal/carrier/marine); grunt uniquely keys the panic
 * pipeline and flee handling off the leader-proximity counters maintained by actor_situation_update (+515 =
 * nearby allies of type 0, +583 = very-close allies of type 0). DEVIATION: the two `(-x & ~x) >> 31` idioms are
 * branchless `(signed char)x > 0` tests (disasm 0x83829420: lbz+extsb); decoded per the established catalog. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_action.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_surprise_type.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level);
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t actor_action_handle_panic_from_surprise(uint16_t actor_index);
extern uint8_t actor_action_handle_panic_from_damage(int actor_index);
extern uint8_t actor_action_handle_panic_from_attached_projectiles(uint16_t actor_index);
extern int actor_action_handle_panic_from_attached_melee_attackers(uint16_t actor_index);
extern uint8_t actor_action_handle_panic_from_burning_to_death(int actor_index);
extern int actor_action_handle_panic_transition(int actor_index, int16_t minimum_panic_level, uint8_t enforced_calm, int16_t force_panic_level);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern unsigned __int8 actor_action_handle_vehicle_entry(int actor_index);
extern int actor_action_handle_vehicle_exit(uint16_t actor_index);
extern uint8_t actor_action_handle_grenade_throwing(int actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern unsigned __int8 actor_action_can_stop_guarding(int actor_index, __int16 guard_investigate_threshold,
        __int16 cower_investigate_threshold);
extern unsigned __int8 actor_action_can_stop_conversing(int actor_index);
extern uint8_t action_flee_blind_panic(int16_t panic_type);
extern unsigned __int8 actor_action_consider_grenade(int actor_index);

void grunt_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* leader-proximity counters from actor_situation_update: +515 = nearby allies of type 0,
     * +583 = very-close allies of type 0 */
    unsigned __int8 leader_nearby = (signed char)actor->situation.area_friends_by_type[0] > 0;
    unsigned __int8 leader_very_close = (signed char)actor->situation.close_friends_by_type[0] > 0;

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    actor_action_handle_surprise(actor_index, _actor_surprise_unprepared_enemy_shooting);

    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_panic_from_surprise(actor_index);
        actor_action_handle_panic_from_damage(actor_index);
        actor_action_handle_panic_from_attached_projectiles(actor_index);
        actor_action_handle_panic_from_attached_melee_attackers(actor_index);
        actor_action_handle_panic_from_burning_to_death(actor_index);
        actor_action_handle_panic_transition(actor_index, _actor_panic_damage, leader_nearby, _actor_panic_surprise);
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
            unsigned __int8 allow_initiative = 1;
            unsigned __int8 force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision)
              && !actor_action_handle_combat_failure(actor_index) )
                actor_action_handle_evasion(actor_index);
            break;
        }

        case actor_action_flee:
            if ( leader_very_close )
            {
                __int16 panic_type = actor->state.action_data.___u0.flee.panic_type;
                if ( panic_type > _actor_panic_none && !action_flee_blind_panic(panic_type) )
                    actor->state.action_data.___u0.flee.done_fleeing = 1;
            }
            if ( actor->state.action_data.___u0.flee.unable_to_flee )
            {
                unsigned __int8 allow_initiative = 1;
                unsigned __int8 force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
                break;
            }
            if ( !actor_action_handle_done_fleeing(actor_index)
              && !actor->state.action_data.___u0.flee.panic_type
              && actor->state.combat_status >= _actor_combat_status_clear_los )
                actor_action_consider_grenade(actor_index);
            break;

        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
        {
            unsigned __int8 allow_initiative = 1;
            unsigned __int8 force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_exit_pursuit(actor_index);
            break;
        }

        case actor_action_guard:
        {
            unsigned __int8 allow_initiative = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
            unsigned __int8 force_decision = 0;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            break;
        }

        case actor_action_vehicle:
            if ( actor->state.action_data.___u0.vehicle.vehicle_entry_done || actor->state.action_data.___u0.vehicle.vehicle_entry_failed )
            {
                unsigned __int8 allow_initiative = 1;
                unsigned __int8 force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            break;

        case actor_action_obey:
            actor_action_handle_combat_status(actor_index, actor->state.action_data.___u0.obey.initiative, actor->state.action_data.___u0.obey.finished);
            break;

        case actor_action_converse:
        {
            /* +476 = conversation index (-1 = none). */
            unsigned __int8 force_decision = 0;
            if ( actor->state.action_data.___u0.converse.failed || actor->external_orders.conversation_index == -1 )
                force_decision = 1;
            unsigned __int8 can_stop_conversing = actor_action_can_stop_conversing(actor_index);
            actor_action_handle_combat_status(actor_index, can_stop_conversing, force_decision);
            break;
        }

        case actor_action_avoid:
            /* word offset 640 = danger_zone.danger_type (DB-verified): no danger type left to
             * avoid, so force a combat-status decision. */
            if ( !actor->danger_zone.danger_type )
            {
                unsigned __int8 allow_initiative = 1;
                unsigned __int8 force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            break;

        default:
            return;
    }
}