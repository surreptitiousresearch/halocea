/* hunter_decide_action @0x83829260 — per-tick action-class dispatch for the "hunter" actor AI. Runs the
 * common initial/pending-command-list housekeeping, then (unless a transition is denied) the
 * berserk-from-damage/attached-projectiles pipeline, berserk_transition, combat_transition, and danger
 * avoidance. Dispatches per action class: failure-related (3/4/6/0xA, one path) and pursuit-related
 * (5/7/8, one path) both fall back to a secondary handler if combat-status declines; conversation-related
 * (0xB numeric-default-style, 0xC converse, 0xD a distinct opaque flag) all eventually defer to
 * combat-status, 0xC additionally stopping the conversation once it fails or its external order is gone.
 *
 * DEVIATION: case 0xC's condition mixes two DIFFERENT "conversation index" fields that happen to look
 * similar in the raw offsets — actor_external_orders.conversation_index (0x14, script-imposed override,
 * already established) and converse_state_data.failed (action_data+4, the runtime converse action's own
 * failure flag, also already established) — restored using their real field names instead of raw offsets. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/converse_state_data.h"
#include "headers/actor_action.h"
#include "headers/actor_berserk_type.h"
#include "headers/blam_data_globals.h"


extern int actor_action_handle_initial_action(int actor_index);
extern uint8_t actor_action_handle_pending_command_list(int actor_index);
extern uint8_t actor_action_deny_transition(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_damage(uint16_t actor_index);
extern uint8_t actor_action_handle_berserking_from_attached_projectiles(int actor_index);
extern int actor_action_handle_berserk_transition(int actor_index, int16_t berserk_level);
extern uint8_t actor_action_handle_combat_transition(int actor_index);
extern int actor_action_handle_danger_avoidance(int actor_index);
extern uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision);
extern uint8_t actor_action_handle_combat_failure(int actor_index);
extern uint8_t actor_action_handle_exit_pursuit(int actor_index);
extern unsigned __int8 actor_action_can_stop_conversing(int actor_index);

void hunter_decide_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_action_handle_initial_action(actor_index);
    actor_action_handle_pending_command_list(actor_index);

    if ( !actor_action_deny_transition(actor_index) )
    {
        actor_action_handle_berserking_from_damage(actor_index);
        actor_action_handle_berserking_from_attached_projectiles(actor_index);
        actor_action_handle_berserk_transition(actor_index, actor_berserk_damage);
        actor_action_handle_combat_transition(actor_index);
        actor_action_handle_danger_avoidance(actor_index);
    }

    unsigned __int8 allow_initiative;
    unsigned __int8 force_decision;

    switch ( actor->state.action )
    {
        case actor_action_fight:
        case actor_action_flee:
        case actor_action_guard:
        case actor_action_charge:
            allow_initiative = 1;
            force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_combat_failure(actor_index);
            return;

        case actor_action_uncover:
        case actor_action_search:
        case actor_action_wait:
            allow_initiative = 1;
            force_decision = 0;
            if ( !actor_action_handle_combat_status(actor_index, allow_initiative, force_decision) )
                actor_action_handle_exit_pursuit(actor_index);
            return;

        case actor_action_obey:
            force_decision = actor->state.action_data.___u0.obey.finished;
            allow_initiative = actor->state.action_data.___u0.obey.initiative;
            break;

        case actor_action_converse:
        {
            converse_state_data *converse_state = &actor->state.action_data.___u0.converse;
            unsigned __int8 conversation_over = converse_state->failed || actor->external_orders.conversation_index == -1;
            allow_initiative = actor_action_can_stop_conversing(actor_index);
            force_decision = conversation_over;
            break;
        }

        case actor_action_avoid:
            if ( actor->danger_zone.danger_type )
                return;
            force_decision = 1;
            allow_initiative = 1;
            break;

        default:
            return;
    }

    actor_action_handle_combat_status(actor_index, allow_initiative, force_decision);
}
