/* crew_decide_action @0x83828B90 — per-tick action-class dispatch for the "crew" actor AI (e.g. covenant
 * crew/leaders). Runs the common initial/pending-command housekeeping and a surprise check, then (unless a
 * transition is denied) the full panic pipeline (surprise/damage/attached-projectile/attached-melee/burning
 * triggers, panic_transition, combat_transition, danger_avoidance) before dispatching on the actor's action
 * class: failure (3/0xA) → combat-status, else combat-failure, else evasion; flee (4) → force combat-status
 * while it still wants to flee (+170), else finish fleeing; pursuit (5/7/8) → exit-pursuit if combat-status
 * declines; guard (6) → combat-status gated by actor_action_can_stop_guarding; vehicle (9) → combat-status only
 * if vehicle_state_data.vehicle_entry_done/.vehicle_entry_failed (+165/+166) is set; 0xB (obey) → combat-status
 * from obey_state_data.finished/.initiative (+158/+161); converse
 * (0xC) → combat-status gated by actor_action_can_stop_conversing, forced when done/no partner (+160, +476);
 * 0xD → force combat-status unless a busy flag (+640) is set. Sibling of engineer_decide_action (which swaps
 * the surprise-panic step for grenade throwing) and carrier_decide_action.
 *
 * DEVIATION: the action_data bytes are resolved to the live union arm selected by each switch case
 * (flee/vehicle/obey/converse).
 * +108 reuses actor_state_data.action. +476 is actor->external_orders.conversation_index (DB-verified;
 * an earlier pass mislabeled it conversation_attention_unit_index, which is +480).
 * +640 is actor->danger_zone.danger_type. The goto-based shared combat-status tail is reflowed into per-case calls. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/data_array.h"
#include "headers/actor_action.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_surprise_type.h"
#include "headers/blam_data_globals.h"


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
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern unsigned __int8 actor_action_can_stop_guarding(int actor_index, __int16 guard_investigate_threshold,
    __int16 cower_investigate_threshold);
extern unsigned __int8 actor_action_can_stop_conversing(int actor_index);

void crew_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

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
        actor_action_handle_panic_transition(actor_index, _actor_panic_damage, 0, _actor_panic_grenade_attached_to_us);
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_danger_avoidance(actor_index);
    }

    unsigned __int8 allow_initiative;
    unsigned __int8 force_decision;

    switch ( actor->state.action )
    {
        case actor_action_fight:
        case actor_action_charge:
            allow_initiative = 1;
            force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision)
              && !actor_action_handle_combat_failure(actor_index) )
                actor_action_handle_evasion(actor_index);
            return;

        case actor_action_flee:
            if ( actor->state.action_data.___u0.flee.unable_to_flee )
            {
                allow_initiative = 1;
                force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
                return;
            }
            actor_action_handle_done_fleeing(actor_index);
            return;

        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
            allow_initiative = 1;
            force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_exit_pursuit(actor_index);
            return;

        case actor_action_guard:
            allow_initiative = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
            force_decision = 0;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            return;

        case actor_action_vehicle:
            if ( actor->state.action_data.___u0.vehicle.vehicle_entry_done || actor->state.action_data.___u0.vehicle.vehicle_entry_failed )
            {
                allow_initiative = 1;
                force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            return;

        case actor_action_obey:
            force_decision = actor->state.action_data.___u0.obey.finished;
            allow_initiative = actor->state.action_data.___u0.obey.initiative;
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            return;

        case actor_action_converse:
            /* conversation_index is +476 (lwz r11, 0x1DC(r30)); a prior pass mis-typed this as
             * conversation_attention_unit_index (+480). */
            force_decision = (actor->state.action_data.___u0.converse.failed || actor->external_orders.conversation_index == -1) ? 1 : 0;
            allow_initiative = actor_action_can_stop_conversing(actor_index);
            actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            return;

        case actor_action_avoid:
            /* word offset 640 = danger_zone.danger_type (DB-verified): in the avoid action,
             * a cleared danger type means there is nothing left to avoid, so force a decision. */
            if ( !actor->danger_zone.danger_type )
            {
                allow_initiative = 1;
                force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            return;

        default:
            return;
    }
}
