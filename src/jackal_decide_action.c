/* jackal_decide_action @0x838296F0 — per-tick action-class dispatch for the "jackal" actor AI (a cowardly,
 * shield-carrying covenant type). Runs the common initial/pending-command-list housekeeping plus a surprise
 * check, then (unless a transition is denied) runs the full panic pipeline (surprise/damage/attached-projectile/
 * melee/burning panic triggers, then panic_transition, then combat_transition, active-cover seeking, vehicle
 * entry/exit and danger avoidance) before dispatching on the current action class (state.action):
 *   3/0xA : force combat-status, else combat-failure, else evasion.
 *   4     : if still "wants to flee" (+170) force combat-status(1,1), else finish fleeing.
 *   5/7/8 : combat-status, else exit-pursuit.
 *   6     : guard — refresh a cower/guard hold timer against the character's guard ranges, then defer to
 *           actor_action_can_stop_guarding for combat-status.
 *   0xB   : combat-status using two actor-state flags (+158, +161) directly.
 *   0xC   : converse — combat-status via actor_action_can_stop_conversing, forcing decision when the actor is
 *           done conversing (+160) or has no conversation (+476 == -1).
 *   0xD   : if no scripted-freeze word (+640) force combat-status(1,1).
 *   default: no-op.
 *
 * DEVIATION: state.action (+108) and state.combat_status (+110) are modeled; the remaining fields
 * (+164/165/166/168/170 in state.action_data, +158/160/161 in state.action_data, the guard-range floats at
 * character_definition +736/+740) fall in actor_datum's / the character tag's unmodeled opaque payload and
 * are accessed via raw offsets, matching the sibling *_decide_action files (carrier/infection). The
 * decompiler's LABEL_28 shared tail (case 4's flee-force and case 0xD both call combat_status(1,1)) is
 * reproduced inline. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_combat_status.h"
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
extern int actor_action_handle_active_cover_seeking(int actor_index, uint8_t allow_panicking, uint8_t force_panicking);
extern unsigned __int8 actor_action_handle_vehicle_entry(int actor_index);
extern int actor_action_handle_vehicle_exit(uint16_t actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern unsigned __int8 actor_action_can_stop_guarding(int actor_index, __int16 guard_investigate_threshold,
        __int16 cower_investigate_threshold);
extern unsigned __int8 actor_action_can_stop_conversing(int actor_index);

void jackal_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character_definition = TAG_GET(actor_definition, actor->meta.definition_index);

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
        actor_action_handle_panic_transition(actor_index, _actor_panic_damage, 0, _actor_panic_no_cover_available);
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_active_cover_seeking(actor_index, 1, 1);
        actor_action_handle_vehicle_entry(actor_index);
        actor_action_handle_vehicle_exit(actor_index);
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
            if ( actor->state.action_data.___u0.flee.unable_to_flee )
            {
                unsigned __int8 allow_initiative = 1;
                unsigned __int8 force_decision = 1;
                actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
            }
            else
                actor_action_handle_done_fleeing(actor_index);
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
            /* the character tag's guard-range floats at +736/+740 are opaque payload. */
            if ( actor->state.action_data.___u0.guard.cower && !actor->state.action_data.___u0.guard.cower_panicked && !actor->state.action_data.___u0.guard.cower_from_retreat )
            {
                float guard_range;
                if ( actor->state.combat_status < _actor_combat_status_certain )
                    guard_range = character_definition->defensive.shield_fraction_emerge_pursue;
                else
                    guard_range = character_definition->defensive.shield_fraction_emerge_attack;

                char keep_guarding;
                if ( actor->emotions.original_body_vitality >= guard_range )
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
                unsigned __int8 can_stop_guarding = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
                unsigned __int8 force_decision = 0;
                actor_action_handle_combat_status(actor_index, can_stop_guarding, force_decision);
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
            /* actor->danger_zone.danger_type == actor_danger_zone_none means no active danger; used here as a scripted-freeze guard. */
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