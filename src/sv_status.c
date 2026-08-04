/* sv_status @0x83765F88 — server console command: print the dedicated server's current map, player count,
 * and whether the game is ending. Server-only; rejected on a client.
 * DEVIATION: the max-player count is read as a signed byte from the absolute address 0x13D (a fixed
 * low-memory config location); reproduced verbatim. */

#include <stdint.h>
#include <stddef.h>
#include "headers/real_argb_color.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_server.h"
extern int16_t game_connection(void);
extern network_game_server *global_network_game_server_get(void);
extern int players_in_game(void);
extern char *main_get_multiplayer_map_name(void);
extern uint8_t game_engine_is_in_end_game_state(void);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_status(void)
{
    if ( game_connection() == _game_connection_network_server )
    {
        if ( global_network_game_server_get() )
        {
            int max_players = *(signed char *)0x13D;
            int player_count = players_in_game();
            const char *multiplayer_map_name = main_get_multiplayer_map_name();
            ship_terminal_printf(
                NULL /* default terminal color */,
                "Dedicated server is running on map %s (%d / %d players)",
                multiplayer_map_name,
                player_count,
                max_players);
            if ( game_engine_is_in_end_game_state() )
                ship_terminal_printf(global_real_argb_red, "Game is ending...");
            else
                ship_terminal_printf(NULL /* default terminal color */, "Use the 'sv_end_game' command to stop the game.");
        }
    }
    else
    {
        ship_terminal_printf(global_real_argb_white, "%s is a server-only function!", "sv_status");
    }
}
