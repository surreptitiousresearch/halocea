/* update_client_queue @ 0x836F5F58 — stash the action the local client produced this frame into the
 * saved-action collection slot for the current local player, record the tick the action applies to, and
 * advance to the next local player. The original copies the 32-byte player_action as eight dwords into
 * saved_action_collection.actions[current_local_player]; expressed here as a struct copy. */

#include "headers/update_client_globals.h"
#include "headers/player_action.h"

void update_client_queue(const player_action *action, int ticks_to_apply_action_to)
{
    update_client_globals.saved_action_collection.actions[update_client_globals.current_local_player] = *action;
    update_client_globals.client_ticks_to_apply_action_to = ticks_to_apply_action_to;
    ++update_client_globals.current_local_player;
}
