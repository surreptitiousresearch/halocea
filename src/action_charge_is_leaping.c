/* action_charge_is_leaping @0x837F9500 — true if the actor is mid-charge (state word +80 == 3), has left
 * the ground (byte +167), and its leap start time (dword +43) plus a 30-tick grace window hasn't yet
 * elapsed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/charge_state_data.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

uint8_t action_charge_is_leaping(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action_data.___u0.charge.goal == charge_goal_melee_leaping && actor->state.action_data.___u0.charge.launched_leap )
        return actor->state.action_data.___u0.charge.leap_start_time + 30 >= game_time_get();

    return 0;
}
