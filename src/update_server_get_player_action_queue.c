/* update_server_get_player_action_queue @0x836F5D20 — return a pointer to a player's action queue inside the
 * update-server's queue data array. Each element is 100 bytes; the action_queue lives at +40 within it. */

#include <stdint.h>
#include "headers/update_server_globals.h"
#include "headers/action_queue.h"
#include "headers/data_array.h"
#include "headers/update_server_queue_datum.h"
#include "headers/blam_data_globals.h"


action_queue *update_server_get_player_action_queue(uint16_t player_index)
{
    return &DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue, player_index)->queue;
}
