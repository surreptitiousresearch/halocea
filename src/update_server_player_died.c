/* update_server_player_died @0x836F5D48 — reset a player's server-side update queues when the player
 * dies. The per-player record is a 100-byte update_server_queue datum: the action queue's ring is
 * emptied and the machine's current action is neutralized (control flags + throttle cleared). */

#include <stdint.h>
#include "headers/simple_circular_queue.h"
#include "headers/update_server_queue_datum.h"
#include "headers/data_array.h"
#include "headers/update_server_globals.h"
#include "headers/blam_data_globals.h"

extern void simple_circular_queue_empty(simple_circular_queue *queue);

void update_server_player_died(int player_index)
{
    /* DEVIATION: decompiler aliased the datum as a simple_circular_queue[] (queues[2]/entries/max_size
     * punning); the writes land on current_action.control_flags and .throttle per the update_server_queue
     * layout — spelled as the typed members (also fixes the 32-bit-pointer-store assumption for x64). */
    update_server_queue *record = DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue, player_index);

    simple_circular_queue_empty(&record->queue.queue);
    record->current_action.control_flags = 0;
    record->current_action.throttle.n[0] = 0.0f;
    record->current_action.throttle.n[1] = 0.0f;
}
