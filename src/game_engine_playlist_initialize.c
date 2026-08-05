/* game_engine_playlist_initialize @0x837526B4 — prepares the multiplayer playlist. It clears the current
 * playlist (and, when forcing a begin, reloads the map cycle). If the playlist ends up empty but a game
 * variant has been specified in the UI, it seeds a single-entry playlist from the current multiplayer map
 * and that variant. Returns whether the playlist is non-empty.
 *
 * The server-get / playlist-clear / mapcycle-load calls are independent no-arg calls (threaded-r3). */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_engine_playlist_s.h"
#include "headers/game_variant.h"


#include "headers/network_game_server.h"
extern network_game_server *global_network_game_server_get(void);
extern void game_engine_playlist_clear(void);
extern void mapcycle_load(void);
extern uint8_t player_ui_game_variant_specified(game_variant *variant);
extern char *main_get_multiplayer_map_name(void);
extern int game_engine_playlist_add_selection(const char *map_name, const char *in_variant_name, game_variant *in_variant);

int game_engine_playlist_initialize(uint8_t force_begin_playlist)
{
    global_network_game_server_get();
    game_engine_playlist_clear();
    if (force_begin_playlist)
        mapcycle_load();

    if (game_engine_playlist.playlist_count)
        return 1;

    game_variant variant;
    if (player_ui_game_variant_specified(&variant))
    {
        const char *map_name = main_get_multiplayer_map_name();
        if (map_name)
        {
            game_engine_playlist_clear();
            game_engine_playlist_add_selection(map_name, nullptr, &variant);
        }
    }

    return game_engine_playlist.playlist_count != 0;
}
