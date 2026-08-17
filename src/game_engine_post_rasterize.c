/* game_engine_post_rasterize @0x83752FD0 */
/* game_engine_post_rasterize 0x83752FD0 — per-frame post-rasterize hook for the multiplayer game engine:
 * dispatches to the in-game or post-game handler by the current engine mode. No-op when no game
 * engine is active.
 * DEVIATION: the beqlr @0x83752FDC and blr @0x83752FFC exits have no r3 def on any path and the
 * sole caller interface_draw_screen @0x836A6BBC ignores r3 — the decompiler's int return was an
 * artifact; void. */

#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern int game_engine_post_rasterize_in_game(void);
extern int game_engine_post_rasterize_post_game(void);

void game_engine_post_rasterize(void)
{
    if ( game_engine )
    {
        if ( game_engine_globals.mode < (unsigned int)game_engine_mode_postgame_rasterize_delay )
            game_engine_post_rasterize_in_game();
        else
            game_engine_post_rasterize_post_game();
    }
}
