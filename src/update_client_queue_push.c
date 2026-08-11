/* update_client_queue_push @0x836F5FA0 — push an empty client action set onto the update-client queue,
 * to apply "nothingness" (no input) for the given number of ticks. */

#include "headers/update_client_globals.h"
#include <string.h>

void update_client_queue_push(int ticks_to_apply_nothingness_to)
{
    update_client_globals.current_local_player = 0;
    memset(&update_client_globals.saved_action_collection, 0, sizeof(update_client_globals.saved_action_collection));
    update_client_globals.client_ticks_to_apply_action_to = ticks_to_apply_nothingness_to;
}
