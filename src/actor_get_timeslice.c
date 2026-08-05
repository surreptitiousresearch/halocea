/* actor_get_timeslice @ 0x8371EB58 — service-timer arbitration: decide whether this actor gets the single
 * expensive "thought" timeslice available per frame. The actor's service timer (meta.service_timer) is aged
 * up by 1 each tick (3 when it is mid-jump in the guard/charge action — action 10 with sub-state 2..5). If no
 * actor has been served yet this frame and this actor's timer has climbed above the previous frame's highest
 * and over 15, it claims the slice (timer reset, time_given_this_frame set, meta.timeslice=1, returns 1).
 * Otherwise it just updates the running current-highest-service-timer and clears meta.timeslice. */

#include <stdint.h>

#include "headers/actor_datum.h"
#include "headers/ai_globals.h"
#include "headers/data_array.h"
#include "headers/actor_action.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"


/* actor_index attested uint16_t: callee normalizes with clrlwi r3,16 @0x8371EB5C.
 * Return stays int: explicit li r3,0/1 but the only caller (actor_update) ignores r3. */
int actor_get_timeslice(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    uint8_t jumping = 0;
    if ( actor->state.action == actor_action_charge )
    {
        int sub_state = actor->state.action_data.___u0.charge.goal;
        if ( sub_state == charge_goal_melee || sub_state == charge_goal_melee_leaping
          || sub_state == charge_goal_vehicle_strafing || sub_state == charge_goal_vehicle_ramming )
            jumping = 1;
    }

    int16_t service_timer = (jumping ? 3 : 1) + actor->meta.service_timer;
    actor->meta.service_timer = service_timer;

    if ( ai_globals->time_given_this_frame
      || service_timer <= ai_globals->last_highest_service_timer
      || service_timer <= 15 )
    {
        if ( actor->meta.service_timer > ai_globals->current_highest_service_timer )
            ai_globals->current_highest_service_timer = actor->meta.service_timer;
        actor->meta.timeslice = 0;
        return 0;
    }

    actor->meta.service_timer = 0;
    ai_globals->time_given_this_frame = 1;
    actor->meta.timeslice = 1;
    return 1;
}
