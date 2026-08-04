/* multiplayer_sound_to_network @0x837B68E8 — encode a "multiplayer sound" message-delta and send it to all
 * (or one) loaded machine(s). Same encode/send idiom as _game_engine_play_multiplayer_sound.c. */

#include <stdint.h>
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/blam_data_globals.h"

extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int  _message_multiplayer_sound;
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void multiplayer_sound_to_network(int player_index, int index)
{
    int message_body = index;
    int size_in_bits = message_delta_processor_encode_stateless(
        _message_multiplayer_sound, 0, &message_body, g_message_encode_buffer, 32760);

    if ( size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();

        if ( player_index == -1 )
        {
            network_game_server_send_message_to_all_loaded_machines(
                server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1u, 0, 0, 3);
        }
    }
}
