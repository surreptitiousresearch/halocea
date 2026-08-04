/* update_client_dequeue_client @0x836F6068 — pop one update tick for each queued client and fill the
 * caller's per-client actions array from the saved action collection. Each output action is a copy of the
 * local machine's saved action (actions[0]), with its control_flags recomputed to the freshly-pressed
 * bits (saved & ~latched). The latched-control mask is updated to the set of held "sticky" buttons
 * (mask 0x4D0). For the first client the tick-application countdown is decremented. Always returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_action.h"
#include "headers/update_client_globals.h"
#include "headers/blam_data_globals.h"


extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

uint8_t update_client_dequeue_client(player_action *actions)
{
    int client_index = -1;
    unsigned int fresh_control_flags =
        update_client_globals.saved_action_collection.actions[0].control_flags
        & ~update_client_globals.latched_control_flags;
    update_client_globals.latched_control_flags =
        update_client_globals.saved_action_collection.actions[0].control_flags & 0x4D0;

    data_iterator iterator;
    data_iterator_new(&iterator, update_client_globals.queues);
    while (data_iterator_next(&iterator))
    {
        ++client_index;
        /* Original copies 8 dwords from the saved action collection base (actions[0]) into
         * actions[client_index]; equivalent to this struct assignment. */
        actions[client_index] = update_client_globals.saved_action_collection.actions[0];
        actions[client_index].control_flags = fresh_control_flags;
        if (!client_index)
            --update_client_globals.client_ticks_to_apply_action_to;
    }
    ++update_client_globals.next_update_number_to_dequeue;
    return 1;
}
