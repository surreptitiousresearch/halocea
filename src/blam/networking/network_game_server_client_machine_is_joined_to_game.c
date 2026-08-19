/* network_game_server_client_machine_is_joined_to_game @0x83745F80 — retail stub: unconditionally returns 0. */

#include <stdint.h>
#include "headers/network_client_machine.h"
#include "headers/network_game_server.h"

uint8_t network_game_server_client_machine_is_joined_to_game(network_game_server *server, network_client_machine *machine)
{
    return 0;
}
