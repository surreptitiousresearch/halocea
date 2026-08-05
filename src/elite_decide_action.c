/* elite_decide_action @0x838299C8 — per-tick action-class dispatch for the "elite" actor AI (an aggressive,
 * berserk-prone covenant leader). Runs the common initial/pending-command-list housekeeping plus a surprise
 * check (minimum level 4), then (unless a transition is denied) runs the berserk pipeline — attacking-mode /
 * damage / proximity / attached-projectile berserk triggers, then berserk_transition — and, only when the elite
 * is not already committed (emotions.berserk flag clear), the attached-projectile/melee panic triggers and
 * panic_transition; followed by combat transition, (non-panicking) active-cover seeking, vehicle entry/exit,
 * grenade throwing and danger avoidance. Then dispatches on the action class (state.action):
 *   3/0xA : force combat-status, else combat-failure, else evasion.
 *   4     : if "wants to flee" (action_data[14]) force combat-status(1,1), else finish fleeing.
 *   5/7/8 : combat-status, else exit-pursuit.
 *   6     : guard — refresh a cower/guard hold timer against the character's guard ranges (+736/+740), then
 *           defer to actor_action_can_stop_guarding for combat-status.
 *   9     : if a guard-state byte (action_data[9]/[10]) is set, force combat-status(1,1).
 *   0xB   : combat-status using two actor-state flags (action_data[2], action_data[5]) directly.
 *   0xC   : converse — combat-status via actor_action_can_stop_conversing, forcing decision when done
 *           conversing (action_data[4]) or with no conversation (external_orders.conversation_index == -1).
 *   0xD   : if no scripted-freeze word (danger_zone first int16) force combat-status(1,1).
 *   default: no-op.
 *
 * Same template as the sibling *_decide_action files (jackal/carrier/grunt); the elite uses the berserk pipeline
 * (like carrier) plus the guard-timer refresh (like jackal). The action_data bytes are resolved to the live
 * union arm selected by each switch case (flee/guard/vehicle/obey/converse); the character tag's guard-range
 * floats are now the named character_definition->defensive.shield_fraction_emerge_pursue/_attack fields. The
 * decompiler's shared LABEL_33 tail (case 4/9/0xD all call
 * combat_status(1,1)) is reproduced inline. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_action.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_surprise_type.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_berserk_type.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level);
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_attacking_mode(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_damage(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_proximity(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_attached_projectiles(int actor_index);
extern int actor_action_handle_berserk_transition(int actor_index, int16_t berserk_level);
extern uint8_t actor_action_handle_panic_from_attached_projectiles(uint16_t actor_index);
extern int actor_action_handle_panic_from_attached_melee_attackers(uint16_t actor_index);
extern int actor_action_handle_panic_transition(int actor_index, int16_t minimum_panic_level, uint8_t enforced_calm, int16_t force_panic_level);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern int actor_action_handle_active_cover_seeking(int actor_index, uint8_t allow_panicking, uint8_t force_panicking);
extern uint8_t actor_action_handle_vehicle_entry(int actor_index);
extern int actor_action_handle_vehicle_exit(uint16_t actor_index);
extern uint8_t actor_action_handle_grenade_throwing(int actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern uint8_t actor_action_can_stop_guarding(int actor_index, int16_t guard_investigate_threshold, int16_t cower_investigate_threshold);
extern uint8_t actor_action_can_stop_conversing(int actor_index);

void elite_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character_definition = TAG_GET(actor_definition, actor->meta.definition_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    actor_action_handle_surprise(actor_index, _actor_surprise_unprepared_grenade);

    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_berserking_from_attacking_mode(actor_index);
        actor_action_handle_berserking_from_damage(actor_index);
        actor_action_handle_berserking_from_proximity(actor_index);
        actor_action_handle_berserking_from_attached_projectiles(actor_index);
        actor_action_handle_berserk_transition(actor_index, actor_berserk_attacking);
        if ( !actor->emotions.berserk )
        {
            actor_action_handle_panic_from_attached_projectiles(actor_index);
            actor_action_handle_panic_from_attached_melee_attackers(actor_index);
            actor_action_handle_panic_transition(actor_index, _actor_panic_grenade_attached_to_us, 0, _actor_panic_melee_attached_to_us);
        }
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_active_cover_seeking(actor_index, 0, 0);
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
            if ( actor->state.action_data.___u0.guard.cower && !actor->state.action_data.___u0.guard.cower_panicked && !actor->state.action_data.___u0.guard.cower_from_retreat )
            {
                float guard_range;
                if ( actor->state.combat_status < _actor_combat_status_certain )
                    guard_range = character_definition->defensive.shield_fraction_emerge_pursue;
                else
                    guard_range = character_definition->defensive.shield_fraction_emerge_attack;

                char keep_guarding;
                if ( actor->input.shield_vitality >= guard_range )
                {
                    keep_guarding = 0;
                    actor->state.action_data.___u0.guard.cower_ticks = 0;
                }
                else
                {
                    keep_guarding = 1;
                    actor->state.action_data.___u0.guard.cower_ticks = 30;
                }
                actor->state.action_data.___u0.guard.cower = keep_guarding;
            }
            {
                uint8_t can_stop_guarding = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
                uint8_t force_decision = 0;
                actor_action_handle_combat_status(actor_index, can_stop_guarding, force_decision);
            }
            break;

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
            uint8_t force_decision = 0;
            if ( actor->state.action_data.___u0.converse.failed || actor->external_orders.conversation_index == -1 )
                force_decision = 1;
            uint8_t can_stop_conversing = actor_action_can_stop_conversing(actor_index);
            actor_action_handle_combat_status(actor_index, can_stop_conversing, force_decision);
            break;
        }

        case actor_action_avoid:
            /* danger_zone.danger_type (+640, first int16 of danger_zone; DB-verified): no danger
             * type left to avoid, so force a combat-status decision. */
            if ( !actor->danger_zone.danger_type )
            {
                uint8_t allow_initiative = 1;
                uint8_t force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            break;

        default:
            return;
    }
}