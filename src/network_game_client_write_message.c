/* network_game_client_write_message @0x83802928 — retail stub: unconditionally returns 0. */

#include <stdint.h>
#include "headers/network_connection.h"
#include "headers/network_message_type.h"

uint8_t network_game_client_write_message(network_connection *connection, network_message_type type, void *message, unsigned int message_size_in_bits, uint8_t reliable, uint8_t immediate)
{
    return 0;
}
