/* action_search_begin @0x83822D20 — arm an actor's "search" action timer: pick a (min,max) bound pair from
 * the actor's character definition (actor->meta.definition_index, opaque float array — one of two pairs
 * selected by an action_data flag word at action_data+8) and set a randomized tick count (seconds * 30)
 * into the DB-named search_state_data arm's search_desired_time / search_remaining_time fields. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/blam_data_globals.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void action_search_begin(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    float lower_bound, upper_bound;
    if ( (uint16_t)actor->state.action_data.___u0.search.pursuit_location.type )
    {
        lower_bound = actor_def->pursuit.pursuit_position_time_lower_bound;
        upper_bound = actor_def->pursuit.pursuit_position_time_upper_bound;
    }
    else
    {
        lower_bound = actor_def->pursuit.target_location_time_lower_bound;
        upper_bound = actor_def->pursuit.target_location_time_upper_bound;
    }

    unsigned int *seed = get_global_random_seed_address();
    int search_time = (int)(real_seed_random_range(seed, lower_bound, upper_bound) * 30.0f);

    actor->state.action_data.___u0.search.search_desired_time = search_time;
    actor->state.action_data.___u0.search.search_remaining_time = search_time;
}
