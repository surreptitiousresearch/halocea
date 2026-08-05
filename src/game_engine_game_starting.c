/* game_engine_game_starting @0x83746268 — notify the active game engine that the game is starting, then
 * build the level lighting.
 *
 * DEVIATION: the decompiler threaded an r3 "status" out of this function; r3 at the blr is only
 * game_engine_build_lighting's residue (and undefined when no engine is active), and the sole caller
 * game_initial_pulse ignores it — attested void. */

#include "headers/game_engine.h"

extern void game_engine_build_lighting(void);

void game_engine_game_starting(void)
{
    if ( game_engine )
    {
        if ( game_engine->game_starting )
            game_engine->game_starting();
        game_engine_build_lighting();
    }
}
