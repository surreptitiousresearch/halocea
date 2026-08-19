/* actor_action_flush_position_indices @0x837F1AB8 — dispatch to the current action type's flush_position_indices
 * handler (if any) in the global action-function table, keyed by the actor's active action (state.action). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>

extern action_specification global_action_functions[];

void actor_action_flush_position_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    void (*flush_position_indices)(int) =
        global_action_functions[actor->state.action].flush_position_indices;
    if ( flush_position_indices )
        flush_position_indices(actor_index);
}
