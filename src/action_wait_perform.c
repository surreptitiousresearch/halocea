/* action_wait_perform @0x838227C8 — per-tick "wait" action update. Bails (returning the current wait_done
 * flag) unless the actor is scheduled to update this tick (meta.timeslice). Otherwise clears desire_move
 * and refreshes nearby-actor pursuit info, then decides — based on whether this actor is the wait
 * coordinator, whether it has a saved path-target prop (external_orders.pursuit_group_prop_index), and
 * that prop's visibility/distance — whether the wait is done and whether the actor should move toward the
 * prop. Finally drives movement: swarm actors (meta.swarm) never move; otherwise it either halts, or
 * moves toward the prop (flagging move_failed if actor_move_to_prop reports no progress).
 *
 * The wait working state is a wait_state_data overlaid on the action_data union at actor->state.action_data. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/wait_state_data.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


extern int actor_pursuit_find_nearby_actors(int actor_index, int pursuit_coordinator);
extern int game_time_get(void);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern uint8_t actor_move_to_prop(int actor_index, unsigned int prop_index, float accept_radius);

unsigned __int8 action_wait_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    wait_state_data *wait = &actor->state.action_data.___u0.wait;

    if ( !actor->meta.timeslice )
        return wait->wait_done;

    wait->desire_move = 0;
    actor_pursuit_find_nearby_actors(actor_index, actor->external_orders.pursuit_is_coordinator);

    int prop_index = actor->external_orders.pursuit_group_prop_index;

    if ( !wait->waiting_as_coordinator )
    {
        wait->wait_done = 1;
        if ( prop_index != -1 )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

            if ( !wait->was_actively_searching || wait->move_failed )
            {
                if ( prop->visibility < 2 || prop->distance >= 8.0f )
                {
                    wait->wait_done = 1;
                }
                else
                {
                    unsigned __int8 desire_move = 1;
                    if ( wait->move_failed || prop->distance <= 3.5f )
                        desire_move = 0;
                    wait->wait_done = 0;
                    wait->desire_move = desire_move;
                }
            }
            else if ( prop->distance <= 3.5f )
            {
                wait->wait_done = 0;
                wait->desire_move = 0;
            }
            else
            {
                wait->wait_done = 0;
                wait->desire_move = 1;
            }
        }
    }
    else if ( prop_index != -1 )
    {
        if ( game_time_get() >= wait->entry_time + 2700 )
            wait->wait_done = 1;
    }
    else if ( !wait->exit_timer )
    {
        wait->exit_timer = 150;
    }

    if ( actor->meta.swarm )
        return wait->wait_done;

    if ( !wait->desire_move )
    {
        actor_move_halt(actor_index);
        return wait->wait_done;
    }

    unsigned __int8 made_progress = actor_move_to_prop(actor_index, prop_index, 8.0f) != 0;
    unsigned __int8 result = wait->wait_done;
    if ( !made_progress )
        wait->move_failed = 1;
    return result;
}
