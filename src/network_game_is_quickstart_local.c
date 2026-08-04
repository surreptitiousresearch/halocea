#include <stdint.h>
#include "headers/blam_data_globals.h"
/* network_game_is_quickstart_local @0x837567F8 — true when hosting a local (non-remote-accepting)
 * server that was started via the "quickstart" flow. */

typedef struct network_game_server network_game_server;

uint8_t network_game_is_quickstart_local(void)
{
    if (!global_network_game_server || accept_remote_connections)
        return 0;

    return quickstart_network_game_active == 1;
}
