/* actor_action_handle_combat_failure @0x837F6728 — while an actor is in the combat action (actor->state.mode
 * action == actor_action_charge), checks whether its combat sub-state (action_data+4) has hit a failure condition — for sub-states 2/3,
 * any of three failure flags (action_data+7, action_data+8, action_data+41); for sub-states 4/5, just
 * action_data+41 — and if so hands off to actor_action_handle_combat_selection to pick a new combat action.
 * Returns whatever that call returns, or 0 if not currently in combat or no failure condition was met.
 *
 * Fields resolved to the DB-named fight/charge arms of action_data (+4 combat sub-state, +7/+8/+41
 * combat-failure flags; see actor_action_set_default_state.c). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"


extern unsigned __int8 actor_action_handle_combat_selection(int actor_index);

uint8_t actor_action_handle_combat_failure(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* Decompiler misread the member: the compare reads offset 0x6C (state+12 = action, an actor_action)
     * not 0x6A (state+10 = mode). Value 10 = actor_action_charge, matching the charge action arm inspected
     * below. Corrected member + named literal. */
    if ( actor->state.action == actor_action_charge )
    {
        unsigned __int8 failed = 0;
        __int16 combat_sub_state = actor->state.action_data.___u0.charge.goal;

        if ( combat_sub_state == charge_goal_melee || combat_sub_state == charge_goal_melee_leaping )
        {
            if ( actor->state.action_data.___u0.charge.finished_melee_attack || actor->state.action_data.___u0.charge.aborted_melee_attack || actor->state.action_data.___u0.charge.unable_to_advance )
                failed = 1;
        }
        else if ( combat_sub_state == charge_goal_vehicle_strafing || combat_sub_state == charge_goal_vehicle_ramming )
        {
            failed = actor->state.action_data.___u0.charge.unable_to_advance;
        }

        if ( failed )
            return actor_action_handle_combat_selection(actor_index);
    }

    return 0;
}
