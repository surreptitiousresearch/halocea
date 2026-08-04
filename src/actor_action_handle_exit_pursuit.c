/* actor_action_handle_exit_pursuit @0x837F67C8 — when leaving a pursuit-family action, report the outcome
 * to perception for the current target prop (dword +624) unless a search is still pending (word +164):
 * action 5 (uncover, flag byte +157) → tried_to_uncover; action 7 (search, flag byte +156) →
 * tried_to_search; action 8 (guard/abandon, flag byte +156) → abandoned_search unconditionally. When a
 * report fired, chain into the lost-contact handler and return its result. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern void actor_perception_tried_to_uncover(int actor_index, int prop_index);
extern void actor_perception_tried_to_search(int actor_index, int prop_index);
extern void actor_perception_abandoned_search(int actor_index, int prop_index);
extern uint8_t actor_action_handle_lost_contact(int actor_index);

uint8_t actor_action_handle_exit_pursuit(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned __int8 reported = 0;

    switch ( actor->state.action )
    {
    case actor_action_uncover:
        reported = actor->state.action_data.___u0.uncover.uncover_done;
        if ( reported && !actor->state.action_data.___u0.uncover.pursuit_location.type )
            actor_perception_tried_to_uncover(actor_index, actor->target.target_prop_index);
        break;
    case actor_action_search:
        reported = actor->state.action_data.___u0.search.search_done;
        if ( reported && !actor->state.action_data.___u0.search.pursuit_location.type )
            actor_perception_tried_to_search(actor_index, actor->target.target_prop_index);
        break;
    case actor_action_wait:
        reported = actor->state.action_data.___u0.wait.wait_done;
        if ( reported )
            actor_perception_abandoned_search(actor_index, actor->target.target_prop_index);
        break;
    }

    return reported ? actor_action_handle_lost_contact(actor_index) : 0;
}
