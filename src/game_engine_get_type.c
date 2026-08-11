/* game_engine_get_type @0x8374CD48 — return the active multiplayer game engine's type, or -1 when there
 * is no game engine (single-player). */

#include "headers/game_engine.h"


unsigned int game_engine_get_type(void)
{
    if (game_engine)
        return game_engine->type;
    return -1;
}
