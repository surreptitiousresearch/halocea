/* game_engine_change_mode_to_network @0x8374A808 — replicate a game-engine mode change to all loaded
 * client machines: encode a stateless "change mode" message-delta and, if encoding produced bits, send
 * it reliably to every machine via the network game server. */

#include <stdint.h>
#include "headers/game_engine_globals.h"
#include "headers/network_message_type.h"
#include "headers/blam_data_globals.h"

typedef struct network_game_server network_game_server;

/* attestation fix: binary passes the id as an immediate (li r3, 0x16 @0x8374A838) — this is a
 * message_delta_processor_message_definition_type constant, not a linked variable. Belongs in
 * headers/message_delta_message_ids.h (orchestrator: fold it in). */
#include "headers/message_delta_message_ids.h"
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

#define MESSAGE_ENCODE_BUFFER_BITS 0x7FF8

void game_engine_change_mode_to_network(game_engine_mode new_mode)
{
    game_engine_mode message_body = new_mode;
    int size_in_bits = message_delta_processor_encode_stateless(
        _message_game_engine_change_mode, 0, &message_body, g_message_encode_buffer, MESSAGE_ENCODE_BUFFER_BITS);
    if ( size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(
            server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
    }
}
