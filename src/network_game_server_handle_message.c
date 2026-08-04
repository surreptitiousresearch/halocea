/* network_game_server_handle_message @0x837565C8 — retail stub: unconditionally returns 0. */

#include <stdint.h>
#include "headers/bitstream_t.h"
#include "headers/network_client_machine.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"

uint8_t network_game_server_handle_message(network_game_server *server, network_client_machine *machine, network_message_type type, bitstream_t *const input_stream)
{
    return 0;
}
