/* actor_get_pursuit_location @0x837F3930 — returns the actor's stored pursuit_location only while running
 * action states 5 or 7 (word offset 54, inside actor_datum's action_data union); the pursuit_location
 * is the search/uncover arm's pursuit_location field, shared at the same offset by both action states. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/pursuit_location.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


pursuit_location * actor_get_pursuit_location(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t action_state = actor->state.action;
    if ( action_state == actor_action_search || action_state == actor_action_uncover )
        /* returned for action 5 uncover / 7 search, which share pursuit_location at this offset */
        return &actor->state.action_data.___u0.search.pursuit_location;
    return 0;
}
