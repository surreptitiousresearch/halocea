/* network_game_server_is_client_local @0x83765CA0 — retail stub: unconditionally returns 0. */

#include <stdint.h>
#include "headers/network_client_machine.h"
#include "headers/network_game_server.h"

uint8_t network_game_server_is_client_local(network_game_server *server, network_client_machine *client_machine)
{
    return 0;
}
