/* network_game_server_send_message_to_network_client_machine @0x837565D8 — retail stub: unconditionally returns 0. */

#include <stdint.h>
#include "headers/network_client_machine.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"

uint8_t network_game_server_send_message_to_network_client_machine(network_game_server *server, network_client_machine *client_machine, network_message_type type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority)
{
    return 0;
}
