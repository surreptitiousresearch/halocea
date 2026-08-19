/* update_client_queue_set @0x836F5FF8 — save the local player's action into the client prediction
 * queue. The disassembly is a compiler-expanded 32-byte struct copy (8 dwords through a stack
 * scratch buffer) of *action into saved_action_collection.actions[0]. */

#include "headers/update_client_globals.h"
#include "headers/player_action.h"

void update_client_queue_set(player_action *action)
{
    update_client_globals.saved_action_collection.actions[0] = *action;
}
