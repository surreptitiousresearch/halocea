/* network_game_server_reject_connected_client_machine @0x83765CB0 — retail stub: unconditionally returns 0. */

#include <stdint.h>
#include "headers/connected_rejection_code.h"
#include "headers/network_client_machine.h"
#include "headers/network_game_server.h"

uint8_t network_game_server_reject_connected_client_machine(network_game_server *server, network_client_machine *client_machine, connected_rejection_code reason)
{
    return 0;
}
