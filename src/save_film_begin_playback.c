/* save_film_begin_playback @0x836EB7A4 — start playing back a saved film into `game`. Resets the saved-
 * film action buffers, reads and decodes the film header (validating the 'blam' game code, version 1, and
 * platform 0), then reads and decodes the network-game-data packet and creates its game objects. Finally
 * it binds local player 0 to the first local (machine_index 0) network player and starts game time.
 * Returns 1 on success; on any read/decode failure the film file is closed and 0 returned. */

#include <stdint.h>
#include "headers/network_game_data.h"
#include "headers/network_player.h"
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/saved_film_header.h"
#include "headers/data_packet_group_definition.h"
#include "headers/data_packet_definition.h"
#include "headers/blam_data_globals.h"

#include "headers/data_packet_group_definition.h"
extern data_packet_definition saved_film_header_packet;

extern void *memset(void *dst, int value, unsigned int size);
extern void network_game_invalidate(network_game_data *game);
extern uint8_t saved_film_read(void *buffer, int16_t *size);
extern uint8_t data_packet_group_decode_packet(data_packet_group_definition *group_definition, void *decoded_packet, char *encoded_packet, int16_t *encoded_packet_size, int16_t *packet_type, int16_t *packet_version, int16_t expected_packet_class);
extern uint8_t network_game_create_game_objects(network_game_data *game);
extern uint8_t network_player_is_valid(network_player *player);
extern void local_player_set_player_index(int16_t local_player_index, int player_index);
extern void game_time_start(void);
extern void game_initial_pulse(void);
extern void ufclose(FILE *stream);

uint8_t save_film_begin_playback(network_game_data *game)
{
    __int16 packet_version = SAVED_FILM_VERSION;
    __int16 header_packet_type = _saved_film_header_packet;
    __int16 game_packet_type[4];
    game_packet_type[0] = _network_game_data_packet;

    global_saved_film.player_count = 0;
    memset(global_saved_film.action_updates, 0, sizeof(global_saved_film.action_updates));

    __int16 header_size = saved_film_header_packet.size;
    network_game_invalidate(game);

    unsigned char header_buffer[16];
    saved_film_header header;
    if (!saved_film_read(header_buffer, &header_size)
        || !data_packet_group_decode_packet(&saved_film_group, &header, header_buffer, &header_size,
                                            &header_packet_type, &packet_version, _saved_film_packet_class))
    {
        global_saved_film.mode = _saved_film_inactive_mode;
        ufclose(global_saved_film.file);
        global_saved_film.file = nullptr;
        return 0;
    }

    if (header.game_code != SAVED_FILM_GAME_CODE) /* 'blam' */
        return 0;
    if (header.version != SAVED_FILM_VERSION)
        return 0;
    if (header.platform)
        return 0;

    __int16 game_data_size = header.network_game_data_encoded_size;
    unsigned char game_data_buffer[2096];
    if (game_data_size <= 0
        || !saved_film_read(game_data_buffer, &game_data_size)
        || !data_packet_group_decode_packet(&saved_film_group, game, game_data_buffer, &game_data_size,
                                            game_packet_type, &packet_version, _saved_film_packet_class)
        || !network_game_create_game_objects(game))
    {
        return 0;
    }

    int player_index = 0;
    for (network_player *player = game->players;
         player->machine_index || !network_player_is_valid(player); ++player)
    {
        if (++player_index >= 32)
        {
            game_time_start();
            game_initial_pulse();
            return 1;
        }
    }

    local_player_set_player_index(game->players[player_index].controller_index, 0);
    game_time_start();
    game_initial_pulse();
    return 1;
}
