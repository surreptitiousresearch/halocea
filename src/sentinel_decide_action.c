/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* sentinel_decide_action @0x83827100 — the per-tick action decision for a Sentinel actor. Runs the common
 * pre-decision handlers (initial action, pending command list, surprise, and — unless a transition is denied —
 * combat transition and danger avoidance), then dispatches on the actor's current action-state word to the
 * matching handler. Combat/uncover states retry combat status, then combat failure/evasion; the flee, guard,
 * search, conversation and berserk states each resolve to a final actor_action_handle_combat_status call whose
 * two flags come from that state's own conditions.
 *
 * Opaque actor fields by raw offset: +108 (int16) action-state enum, +158/+160/+161/+170 per-state flags,
 * +476 (int) conversation prop index, +640 (int16) berserk gate. The tangled decompiler gotos (all funnelling
 * to actor_action_handle_combat_status(actor_index, arg2, arg3)) are inlined per case. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_surprise_type.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level);
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_evasion(int actor_index);
extern uint8_t actor_action_handle_done_fleeing(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern uint8_t actor_action_can_stop_guarding(int actor_index, int16_t guard_investigate_threshold, int16_t cower_investigate_threshold);
extern uint8_t actor_action_can_stop_conversing(int actor_index);

void sentinel_decide_action(int actor_index)
{
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);
    actor_action_handle_surprise(actor_index, _actor_surprise_unprepared_enemy_shooting);
    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_danger_avoidance(actor_index);
    }

    uint8_t can_stop;
    uint8_t no_partner;

    switch ( actor->state.action )
    {
        case actor_action_fight:
        case actor_action_charge:
            can_stop = 1;
            no_partner = 0;
            if ( !actor_action_handle_combat_status(actor_index, can_stop, no_partner) && !actor_action_handle_combat_failure(actor_index) )
                actor_action_handle_evasion(actor_index);
            return;

        case actor_action_flee:
            if ( actor->state.action_data.___u0.flee.unable_to_flee )
            {
                can_stop = 1;
                no_partner = 1;
                actor_action_handle_combat_status(actor_index, can_stop, no_partner);
            }
            else
                actor_action_handle_done_fleeing(actor_index);
            return;

        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
            can_stop = 1;
            no_partner = 0;
            if ( !actor_action_handle_combat_status(actor_index, can_stop, no_partner) )
                actor_action_handle_exit_pursuit(actor_index);
            return;

        case actor_action_guard:
            can_stop = actor_action_can_stop_guarding(actor_index, _actor_combat_status_definite, _actor_combat_status_dangerous);
            no_partner = 0;
            actor_action_handle_combat_status(actor_index, can_stop, no_partner);
            return;

        case actor_action_obey:
            actor_action_handle_combat_status(actor_index,
                actor->state.action_data.___u0.obey.initiative,
                actor->state.action_data.___u0.obey.finished);
            return;

        case actor_action_converse:
            no_partner = actor->state.action_data.___u0.converse.failed
                       || actor->external_orders.conversation_index == -1;
            can_stop = actor_action_can_stop_conversing(actor_index);
            actor_action_handle_combat_status(actor_index, can_stop, no_partner);
            return;

        case actor_action_avoid:
            if ( actor->danger_zone.danger_type )
                return;
            can_stop = 1;
            no_partner = 1;
            actor_action_handle_combat_status(actor_index, can_stop, no_partner);
            return;

        default:
            return;
    }
}
