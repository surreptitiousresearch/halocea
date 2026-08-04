/* sv_map_restart @0x83765D30 — server console command: restart the current map by closing UI, stepping
 * the playlist back one, ending the game and closing the console. Rejected off the server.
 *
 * DEVIATION: the decompiler threads r3 between the four teardown calls as if chained; the disassembly
 * sets up no arguments between them — they are independent no-argument calls. */

#include <stdint.h>
#include "headers/real_argb_color.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);
extern void ui_widgets_close_all(void);
extern void *game_engine_playlist_prev(void);
extern void game_engine_end_game(void);
extern void console_close(void);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_map_restart(void)
{
    if ( game_connection() == _game_connection_network_server )
    {
        ui_widgets_close_all();
        game_engine_playlist_prev();
        game_engine_end_game();
        console_close();
    }
    else
    {
        ship_terminal_printf(global_real_argb_white, "sv_map_restart is a server-only function!");
    }
}
