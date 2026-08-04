/* action_wait_update @0x83822978 — per-tick bookkeeping for the "wait" action's three independent
 * countdowns, resolved to the DB-named wait_state_data arm of the action_data union:
 *  - wait.query_timer (+12): on reaching 0, fires an AI communication event (type 17) for the actor's
 *    unit and reseeds itself to a random 300-600 tick count.
 *  - wait.exit_timer (+14): on reaching 0, fires an AI communication event (type 20) for the actor's
 *    unit if wait.waiting_as_coordinator is set, then marks wait.wait_done true.
 *  - wait.look_timer (+16): decremented while wait.desire_move is clear. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/wait_state_data.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

void action_wait_update(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    wait_state_data *wait = &actor->state.action_data.___u0.wait;

    __int16 reseed_timer = wait->query_timer;
    if ( reseed_timer > 0 )
    {
        wait->query_timer = reseed_timer - 1;
        if ( reseed_timer == 1 )
        {
            int unit_index = actor->meta.unit_index;
            if ( unit_index != -1 )
                ai_communication_event(_ai_communication_search_query, unit_index, -1, -1, -1, -1, nullptr);

            wait->query_timer = seed_random_range(get_global_random_seed_address(), 300, 600);
        }
    }

    __int16 finish_timer = wait->exit_timer;
    if ( finish_timer > 0 )
    {
        __int16 remaining = finish_timer - 1;
        wait->exit_timer = remaining;
        if ( !remaining )
        {
            if ( wait->waiting_as_coordinator )
            {
                int unit_index = actor->meta.unit_index;
                if ( unit_index != -1 )
                    ai_communication_event(_ai_communication_search_group_abandon, unit_index, -1, -1, -1, -1, nullptr);
            }
            wait->wait_done = 1;
        }
    }

    if ( !wait->desire_move )
    {
        __int16 skip_timer = wait->look_timer;
        if ( skip_timer > 0 )
            wait->look_timer = skip_timer - 1;
    }
}
