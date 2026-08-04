/* save_film_begin_recording @0x836EB278 — start recording a saved film (demo) for this session, if enabled.
 * Captures the network game state (the real one when networked, or a synthesized single-player session
 * otherwise), opens "d:\<map>.flm", and writes the encoded game header and local-player packets. On any failure
 * the film file is closed and recording is left off.
 *
 * Deviation: the DB types this as returning _iobuf*, but the binary's returned value is unused dead state; this
 * reconstruction returns void. */

#include <stdint.h>
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/network_game_data.h"
#include "headers/network_player.h"
#include "headers/data_packet_group_definition.h"
#include <stdio.h>


#include "headers/data_packet_group_definition.h"
extern uint8_t game_in_editor(void);
extern int16_t game_connection(void);
extern network_game_data *network_game_get_game(void);
extern void network_game_invalidate(network_game_data *game);
extern char *main_get_map_name(void);
extern char *strrchr(const char *string, int ch);
extern uint32_t get_random_seed(void);
extern int16_t game_difficulty_level_get(void);
extern uint8_t network_game_add_player(network_game_data *game, network_player *player);
extern uint8_t data_packet_group_encode_packet(data_packet_group_definition *group_definition, void *decoded_packet, unsigned char *encoded_packet, int16_t *encoded_packet_size, int16_t packet_type, int16_t packet_version);
extern uint8_t saved_film_write(void *buffer, int16_t size);
extern void ufclose(FILE *stream);
extern int sprintf_0(char *string, const char *format, ...);

#include "headers/saved_film_header_packet.h"
#include "headers/game_engine_type.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern void *memset(void *dst, int value, unsigned int n);
extern char *strncpy(char *dst, const char *src, unsigned int n);

void save_film_begin_recording(void)
{
    char filename[128];
    char added_player = 0;
    network_player local_player;
    network_game_data game_data;
    unsigned char encoded_game[2096];

    filename[0] = 0;
    memset(&filename[1], 0, 0x7F);

    if ( !hcex_allow_saved_film_recoring )
        return;

    global_saved_film.player_count = 0;
    memset(global_saved_film.action_updates, 0, sizeof(global_saved_film.action_updates));

    if ( game_in_editor() )
        return;

    if ( game_connection() )
    {
        if ( (unsigned int)game_connection() < _game_connection_film_playback )
        {
            memcpy(&game_data, network_game_get_game(), sizeof(game_data));
            added_player = 1;
        }
    }
    else
    {
        memset(&local_player, 0, sizeof(local_player));
        network_game_invalidate(&game_data);
        strncpy(game_data.map.name, main_get_map_name(), 0x7F);
        game_data.map.name[127] = 0;
        game_data.variant.game_engine_index = game_engine_none;
        game_data.network_game_random_seed = get_random_seed();
        game_data.difficulty_level = game_difficulty_level_get();
        added_player = network_game_add_player(&game_data, &local_player);
    }

    if ( !added_player )
        return;

    char *map_leaf = strrchr(main_get_map_name(), '\\');
    sprintf_0(filename, "%s%s.%s", "d:\\", map_leaf + 1, "flm");
    game_data.local_data.game_objects_loaded = 0;
    game_data.local_data.pad3[0] = 0;
    game_data.local_data.pad3[1] = 0;
    game_data.local_data.pad3[2] = 0;

    global_saved_film.file = fopen(filename, "wb");
    if ( !global_saved_film.file )
    {
        global_saved_film.next_expected_update &= (added_player != 0) - 1;
        return;
    }

    saved_film_header_packet header;
    /* recovered: header.signature/field_a/field_b -> game_code/version/platform */
    header.game_code = SAVED_FILM_GAME_CODE; /* 'blam' */
    header.version = SAVED_FILM_VERSION;
    header.platform = 0;

    __int16 game_encoded_size = 2048;
    __int16 header_size[3];
    header_size[0] = 16;

    if ( data_packet_group_encode_packet(&saved_film_group, &game_data, encoded_game, &game_encoded_size,
                                         _network_game_data_packet, SAVED_FILM_VERSION)
      && game_encoded_size > 0
      && data_packet_group_encode_packet(&saved_film_group, &header, (unsigned char *)&local_player,
                                         header_size, _saved_film_header_packet, SAVED_FILM_VERSION) )
    {
        global_saved_film.mode = _saved_film_recording_mode;
        if ( saved_film_write(&local_player, header_size[0]) && saved_film_write(encoded_game, game_encoded_size) )
        {
            added_player = 1;
        }
        else
        {
            ufclose(global_saved_film.file);
            global_saved_film.file = 0;
        }
    }

    global_saved_film.next_expected_update &= (added_player != 0) - 1;
}
