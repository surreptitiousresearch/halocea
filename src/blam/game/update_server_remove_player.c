/* update_server_remove_player @0x836F5CC0 — drop a player's server-side update state: shut down their
 * pending action queue (embedded at queue datum +40, records stride 100) and free the datum. */

#include "headers/update_server_globals.h"
#include "headers/action_queue.h"
#include "headers/data_array.h"
#include "headers/update_server_queue_datum.h"

extern void action_queue_shutdown(action_queue *queue);
extern void datum_delete(data_array *data, int index);

void update_server_remove_player(int player_index)
{
    action_queue_shutdown(&DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue, player_index)->queue);
    datum_delete(update_server_globals.queues, player_index);
}
