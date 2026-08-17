/* update_client_build_client_update @0x836F61B8 */
#include <string.h>
#include "headers/update_client_globals.h"

void update_client_build_client_update(player_action_collection *action_collection)
{
    memcpy(action_collection,
           &update_client_globals.saved_action_collection,
           sizeof(player_action_collection));
}
