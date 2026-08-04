/* player_handle_powerup_to_network @0x836AA410 — replicate a timed powerup pickup to all loaded client
 * machines. Builds the message body (network-translated player index, powerup type, duration), encodes it
 * as a stateless message-delta, and if encoding produced bits sends it reliably to every machine. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_player_handle_powerup.h"
#include "headers/blam_data_globals.h"

typedef struct network_game_server network_game_server;

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int  _message_player_handle_powerup;
extern int  network_message_type_message_delta;
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

#define MESSAGE_ENCODE_BUFFER_BITS 0x7FF8

void player_handle_powerup_to_network(int player_index, int16_t powerup_type, int16_t duration_in_ticks)
{
    s_player_handle_powerup_message message_body;
    message_body.player_index =
        field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    message_body.powerup_type = powerup_type;
    message_body.duration_in_ticks = duration_in_ticks;

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_player_handle_powerup, 0, &message_body, g_message_encode_buffer, MESSAGE_ENCODE_BUFFER_BITS);
    if (size_in_bits > 0)
    {
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(
            server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
    }
}
