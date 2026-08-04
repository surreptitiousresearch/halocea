/* game_engine_nonplayer_post_rasterize @0x837515E4 — after the scene has been rasterized but outside any
 * player window, run the postgame presentation: the engine's post-rasterize postgame pass, then for each
 * of the (up to two) local players draw the postgame UI widgets over the full screen and clear any
 * controller vibration. Only runs once the game engine has entered the postgame-rasterize-delay state.
 *
 * DEVIATION: the decompiler collapsed the four halfword stores building the full-screen window bounds into
 * a single 64-bit constant (31457920); the disassembly shows the rectangle is {y0=0, x0=0, y1=480, x1=640}. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/rectangle2d.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern void game_engine_post_rasterize_post_game(void);
extern void render_ui_widgets_postgame(int16_t local_player_index, rectangle2d *window_bounds);
extern void vibrate_player_clear(int16_t local_player_index);

void game_engine_nonplayer_post_rasterize(void)
{
    if (!game_engine || (unsigned int)game_engine_globals.mode < (unsigned int)game_engine_mode_postgame_rasterize_delay)
        return;

    game_engine_post_rasterize_post_game();

    rectangle2d window_bounds = { 0, 0, 480, 640 };
    for (int local_player_index = 0; local_player_index < 2; local_player_index++)
    {
        render_ui_widgets_postgame(local_player_index, &window_bounds);
        vibrate_player_clear(local_player_index);
    }
}
