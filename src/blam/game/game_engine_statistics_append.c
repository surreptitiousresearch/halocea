/* game_engine_statistics_append @0x83746378 — dispatches to the active game engine's statistics_append
 * callback, if any. */

#include "headers/game_engine.h"


void game_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics)
{
    if (game_engine && game_engine->statistics_append)
        game_engine->statistics_append(permanent_statistics, game_statistics);
}
