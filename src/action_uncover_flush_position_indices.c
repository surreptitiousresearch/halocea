/* action_uncover_flush_position_indices @0x83825068 — invalidates the actor's "uncover" action position
 * index and marks its uncover state dirty (word offset 82/83, byte offset 157 — inside actor_datum's
 * action_data union, same layout family as action_flee_flush_position_indices). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/pursuit_location_type.h"
#include "headers/blam_data_globals.h"


void action_uncover_flush_position_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.action_data.___u0.uncover.pursuit_location.type == _pursuit_location_position )
    {
        actor->state.action_data.___u0.uncover.pursuit_location.firing_position_index = -1;
        actor->state.action_data.___u0.uncover.uncover_done = 1;
    }
}
