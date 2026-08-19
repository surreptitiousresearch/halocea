/* actor_update @ 0x83721AA8 — full per-tick update for one active actor. After the general/perception/
 * situation/emotion passes, it clears the per-tick action-output scratch, then runs the action arbitration
 * loop (up to 10 rounds of decide_action / action_perform, breaking once an action is performed and the
 * action did not change, or the iteration cap is hit) and commits the chosen action. For a non-dormant,
 * non-swarm actor it snapshots the current movement/orientation vectors, resets throttle, and drives the
 * control/communication/conversation/destination/look/move/combat/unit-control chain; a swarm actor is
 * driven through actor_type_swarm_control instead.
 *
 * Offsets recovered against disasm at 0x83721B04-0x83721C84: actor+6 -> meta.swarm, actor+19 (meta+0xF) ->
 * meta.dormant; every such access goes through the named actor_meta_data member here.
 *
 * Deviation: the decompiler loaded the 9-dword movement snapshot (src +0x174, dst +0x6FC) into registers in
 * a scrambled order; disasm confirms it is a plain sequential 9-dword copy, reconstructed as the three
 * real_vector3d assignments below. global_zero_vector3d is a pointer global (lwz at 0x83721C5C), dereferenced. */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/action_state_data.h"
extern uint8_t actor_general_update(int actor_index);
extern int actor_get_timeslice(int actor_index);
extern void actor_input_update(int actor_index);
extern void actor_perception_update(int actor_index);
extern void actor_situation_update(int actor_index);
extern void actor_emotion_update(int actor_index);
extern void actor_type_decide_action(int actor_index);
extern void actor_stimulus_clear(int actor_index);
extern uint8_t actor_action_perform(int actor_index);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);
extern void actor_action_update(int actor_index);
extern void actor_type_swarm_control(int actor_index);
extern void actor_action_control(int actor_index);
extern void actor_communication_update(int actor_index);
extern void actor_conversation_control(int actor_index);
extern void actor_destination_update(int actor_index);
extern void actor_look_affect_movement(int actor_index);
extern void actor_move_update(int actor_index);
extern void actor_look_update(int actor_index);
extern void actor_combat_update(int actor_index);
extern void actor_unit_control(int actor_index);

void actor_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor_general_update(actor_index) )
        return;

    actor_get_timeslice(actor_index);
    actor_input_update(actor_index);
    actor_perception_update(actor_index);
    actor_situation_update(actor_index);
    actor_emotion_update(actor_index);

    /* clear the per-tick action-output scratch */
    memset(&actor->orders, 0, 0x84);
    actor->orders.move.animation.impulse          = -1;
    actor->orders.move.override_movement_type     = -1;
    actor->orders.move.override_movement_facing   = -1;

    /* action arbitration loop */
    int  action_performed = 0;
    int  iteration = 0;
    while ( 1 )
    {
        actor->state.action_changed = 0;             /* action-changed flag, cleared each round */
        ++iteration;
        actor_type_decide_action(actor_index);
        actor_stimulus_clear(actor_index);

        if ( (action_performed && !actor->state.action_changed) || iteration >= 10 )
        {
            actor_action_change(actor_index, 0, NULL);
            break;
        }
        action_performed = actor_action_perform(actor_index);
        if ( !action_performed && !actor->state.action_changed )
            break;
    }
    actor_action_update(actor_index);

    if ( actor->meta.dormant )
        return;

    if ( actor->meta.swarm )
    {
        actor_type_swarm_control(actor_index);
        return;
    }

    actor->output.analog_primary_trigger = 0.0f;

    /* snapshot current movement/orientation vectors into the output "previous" slot */
    /* recovered: 9-dword int* copy (out+0x6FC..0x720) -> output.{facing,aiming,looking}_vector */
    actor->output.facing_vector  = actor->input.facing_vector;
    actor->output.aiming_vector  = actor->input.aiming_vector;
    actor->output.looking_vector = actor->input.looking_vector;

    actor->output.control_flags = 0;
    actor->output.throttle = *global_zero_vector3d;   /* throttle reset */
    actor->output.animation.impulse = -1;

    actor_action_control(actor_index);
    actor_communication_update(actor_index);
    actor_conversation_control(actor_index);
    actor_destination_update(actor_index);
    actor_look_affect_movement(actor_index);
    actor_move_update(actor_index);
    actor_look_update(actor_index);
    actor_combat_update(actor_index);
    actor_unit_control(actor_index);
}
