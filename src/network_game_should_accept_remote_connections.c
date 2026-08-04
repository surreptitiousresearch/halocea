#include <stdint.h>
#include "headers/blam_data_globals.h"
/* network_game_should_accept_remote_connections @0x837567A8 — getter for the flag set by
 * network_game_accept_remote_connections. */


uint8_t network_game_should_accept_remote_connections(void)
{
    return accept_remote_connections;
}
