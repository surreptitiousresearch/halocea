/* sv_map @0x83766DB0 — console command to change map/variant. Validates the map+variant against the
 * playlist. On a running server (connection 2) it replaces the playlist with the single selection, resets
 * it, closes UI, and ends the current game. Off-network it starts a fresh server on that map/variant. As a
 * client (other connection) it's rejected. Invalid input prints an error.
 * DEVIATION: the playlist_reset / ui_widgets_close_all / game_engine_end_game / console_close sequence is a
 * chain of independent no-arg calls (threaded-r3 artifact), not nested calls. */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/real_argb_color.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_playlist_verify(const char *map_name, const char *variant_name);
extern int16_t game_connection(void);
extern void game_engine_playlist_clear(void);
extern int game_engine_playlist_add_selection(const char *map_name, const char *in_variant_name, game_variant *in_variant);
extern void game_engine_playlist_reset(void);
extern void ui_widgets_close_all(void);
extern void game_engine_end_game(void);
extern void console_close(void);
extern void main_set_multiplayer_map_name(const char *name);
extern uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant);
extern void player_ui_set_game_variant(game_variant *variant);
extern uint8_t sv_start_new_server_with_name_and_password_from_profile(uint8_t force_begin_playlist);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_map(const char *mapname, const char *variantname)
{
    if ( mapname && variantname && game_engine_playlist_verify(mapname, variantname) )
    {
        if ( game_connection() == _game_connection_network_server )
        {
            game_engine_playlist_clear();
            game_engine_playlist_add_selection(mapname, variantname, 0);
            game_engine_playlist_reset();
            ui_widgets_close_all();
            game_engine_end_game();
            console_close();
        }
        else if ( game_connection() )
        {
            ship_terminal_printf(global_real_argb_white, "sv_map is a server-only function!");
        }
        else
        {
            game_variant variant;
            main_set_multiplayer_map_name(mapname);
            game_engine_get_variant_by_name(variantname, &variant);
            player_ui_set_game_variant(&variant);
            if ( sv_start_new_server_with_name_and_password_from_profile(0) )
                console_close();
        }
    }
    else
    {
        ship_terminal_printf(global_real_argb_red, "sv_map specified invalid map or game variant");
    }
}
