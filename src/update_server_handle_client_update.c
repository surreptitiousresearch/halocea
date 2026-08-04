/* update_server_handle_client_update @0x836F6150 — on the server, deliver a client's per-tick actions into
 * the per-player action queues. Iterates the machine's (up to 2) local-player slots; for each valid player
 * it writes that player's action (action_collection->actions[0], advancing the collection by one action per
 * delivered player) into the player's action queue (update_server_globals.queues datum +40, stride 100).
 * DEVIATION: action_queue_write takes the 32-byte player_action by value (passed in r4-r7); the decompiler
 * rendered this as LODWORD field shuffles. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/action_queue.h"
#include "headers/player_action_collection.h"
#include "headers/update_server_globals.h"
#include "headers/update_server_queue_datum.h"

extern int *machine_get_player_list(int machine_index);
extern uint8_t action_queue_write(action_queue *queue, player_action action, int client_update_id, int ticks_to_apply_update_to);

void update_server_handle_client_update(int machine_index, player_action_collection *action_collection, int ticks_to_apply_update_to, int client_update_id)
{
    int *player_list = machine_get_player_list(machine_index);
    int delivered = 0;
    for ( int slot = 2; slot; --slot, ++player_list )
    {
        if ( *player_list != -1 )
        {
            action_queue *queue = &DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue, *player_list)->queue;
            /* recovered: (char *)action_collection + 32*delivered -> action_collection->actions[delivered] (stride 32 == sizeof(player_action)) */
            action_queue_write(queue, action_collection->actions[delivered], client_update_id, ticks_to_apply_update_to);
            ++delivered;
        }
    }
}
