#include <stdint.h>
#include "headers/blam_data_globals.h"
/* network_game_is_splitscreen_local @0x837567B8 — true when hosting a server that isn't accepting remote
 * connections (i.e. every client is a local splitscreen player). */

typedef struct network_game_server network_game_server;

uint8_t network_game_is_splitscreen_local(void)
{
    if ( !global_network_game_server )
        return 0;
    return !accept_remote_connections;
}
