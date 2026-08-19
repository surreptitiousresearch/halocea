#include <stdint.h>
#include "headers/blam_data_globals.h"
/* network_game_accept_remote_connections @0x83756798 — set the global flag controlling whether the server
 * accepts new remote client connections. */


void network_game_accept_remote_connections(uint8_t accept)
{
    accept_remote_connections = accept;
}
