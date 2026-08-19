/* update_server_add_player @0x836F5C68 — allocate a server update queue for a newly-joined player
 * at a fixed datum index and initialize its action queue. Mirrors the datum idiom used in
 * update_server_start. */

#include "headers/update_server_globals.h"
#include "headers/action_queue.h"
#include "headers/data_array.h"
#include "headers/update_server_queue_datum.h"

extern int datum_new_at_index(data_array *data, int index);
extern void action_queue_initialize(action_queue *queue);

void update_server_add_player(int player_index)
{
    unsigned short queue_index = datum_new_at_index(update_server_globals.queues, player_index);
    action_queue_initialize(&DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue, queue_index)->queue);
}
