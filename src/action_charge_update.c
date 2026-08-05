/* action_charge_update @0x837F9288 — advances an actor's melee-charge burst counter (action_data+14) while it
 * is mid-charge (action_data+4 == 3), has a valid charge target (action_data+11), and is neither
 * blocked (action_data+6) nor already at the charge-attack limit (actor->input.in_midair / byte 348).
 * Fields resolved to the DB-named charge_state_data arm (goal/launched_leap/launched_melee_attack/
 * leap_failure_timer); byte 348 is actor->input.in_midair. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/charge_state_data.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"


void action_charge_update(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ((uint16_t)actor->state.action_data.___u0.charge.goal == charge_goal_melee_leaping && actor->state.action_data.___u0.charge.launched_leap && !actor->state.action_data.___u0.charge.launched_melee_attack && !actor->input.in_midair)
        ++actor->state.action_data.___u0.charge.leap_failure_timer;
}
