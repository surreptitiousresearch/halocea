/* actor_flush_position_indices @0x8371E170 — invalidate an actor's cached spatial-partition indices. Clears the
 * primary position index, and when the cache kind (word 566) is a firing-position or move-position
 * destination clears the secondary cache slot and resets the kind. Delegates to actor_action_flush_position_indices for the action layer. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"

extern void actor_action_flush_position_indices(uint16_t actor_index);

void actor_flush_position_indices(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    __int16 cache_kind = actor->control.path.destination_orders.destination_type;
    actor->firing_positions.current_position_index = -1;
    if ( cache_kind == _destination_firing_position || cache_kind == _destination_move_position )
    {
        actor->control.path.destination_orders.ignore_target_object_index = -1;
        actor->control.path.destination_orders.destination_type = _destination_none;
    }
    actor_action_flush_position_indices(actor_index);
}
