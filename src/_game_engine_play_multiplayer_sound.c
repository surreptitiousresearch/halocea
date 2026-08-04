/* _game_engine_play_multiplayer_sound @0x837B6960 — plays a multiplayer announcer/UI sound for a player,
 * optionally replicating it to clients. The sound to play is looked up in the game globals' multiplayer
 * information sound table by index. When replicating (and we are the server), it encodes and sends a
 * "multiplayer sound" message-delta to all machines. The sound is then played locally if it is destined
 * for everyone (player_index == -1), or for the local player when we are a client of an active game. */

#include <stdint.h>
#include "headers/game_globals_definition.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int  _message_multiplayer_sound;
extern int  network_message_type_message_delta;
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern int16_t game_connection(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void _game_engine_play_multiplayer_sound(int player_index, int index, uint8_t should_replicate)
{
    game_globals_multiplayer_information *multiplayer_info =
        (game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address;
    if (!multiplayer_info || index >= multiplayer_info->sounds.count)
        return;

    tag_reference *sound = &((tag_reference *)multiplayer_info->sounds.address)[index];
    if (!sound)
        return;
    int sound_tag_index = sound->index;
    if (sound_tag_index == -1)
        return;

    if (should_replicate == 1)
    {
        int message_body = index;
        int size_in_bits = message_delta_processor_encode_stateless(
            _message_multiplayer_sound, nullptr, &message_body, g_message_encode_buffer, 32760);
        if (size_in_bits > 0)
        {
            network_game_server *server = global_network_game_server_get();
            if (player_index == -1)
            {
                network_game_server_send_message_to_all_loaded_machines(
                    server, network_message_type_message_delta, g_message_encode_buffer,
                    size_in_bits, 1, 0, 0, 3);
                goto play_sound;
            }
            goto connection_check;   /* replicated, but to a specific player */
        }
    }
    else if (player_index == -1)
    {
        goto play_sound;
    }

connection_check:
    if (game_connection() != _game_connection_network_server)
        goto play_sound;
    {
        player_datum *player = datum_try_and_get(player_data, player_index);
        /* sentinel: local_player_index is __int16; keep the unsigned cast so -1 compares as 0xFFFF */
        if (player && (unsigned __int16)player->local_player_index != 0xFFFF)
            goto play_sound;
    }
    return;

play_sound:
    unspatialized_impulse_sound_new(sound_tag_index, 1.0f);
}
