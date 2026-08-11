/* sv_end_game @0x83765D78 — server console command: force the current game to end. Rejected off the
 * server. */

#include <stdint.h>
#include "headers/real_argb_color.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);
extern void game_engine_end_game(void);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_end_game(void)
{
    if ( game_connection() == _game_connection_network_server )
    {
        gForceServerToStop = 1;
        game_engine_end_game();
        ship_terminal_printf(global_real_argb_white, "Server is stopping the game...");
    }
    else
    {
        ship_terminal_printf(global_real_argb_white, "sv_end_game is a server-only function!");
    }
}
