/* game_engine_post_rasterize 0x83752FD0 — per-frame post-rasterize hook for the multiplayer game engine:
 * dispatches to the in-game or post-game handler by the current engine mode. No-op (returns the incoming r3,
 * undefined) when no game engine is active. */

#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern int game_engine_post_rasterize_in_game(void);
extern int game_engine_post_rasterize_post_game(void);

int game_engine_post_rasterize()
{
    int result;
    if ( game_engine )
    {
        if ( game_engine_globals.mode < (unsigned int)game_engine_mode_postgame_rasterize_delay )
            return game_engine_post_rasterize_in_game();
        return game_engine_post_rasterize_post_game();
    }
    return result;
}
