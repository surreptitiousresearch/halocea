/* game_engine_force_single_screen @0x837477A4 — true when the active game engine is in a postgame
 * rasterize phase (mode in [postgame_rasterize_delay, postgame_rasterize]), during which the renderer is
 * forced to a single (non-split) screen. */

#include <stdint.h>
#include "headers/game_engine_globals.h"

extern void *game_engine;

uint8_t game_engine_force_single_screen(void)
{
    if ( game_engine
         && game_engine_globals.mode >= game_engine_mode_postgame_rasterize_delay )
        return game_engine_globals.mode <= game_engine_mode_postgame_rasterize;
    return 0;
}
