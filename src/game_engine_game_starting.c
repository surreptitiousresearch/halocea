/* game_engine_game_starting @0x83746268 — notify the active game engine that the game is starting, then
 * build the level lighting.
 *
 * DEVIATION: the decompiler leaves the return value uninitialized when no engine is active; initialized
 * to 0 here for definedness. */

#include "headers/game_engine.h"

extern void *game_engine_build_lighting(void);

int game_engine_game_starting(void)
{
    if ( game_engine )
    {
        if ( game_engine->game_starting )
            game_engine->game_starting();
        return (int)game_engine_build_lighting();
    }
    return 0;
}
