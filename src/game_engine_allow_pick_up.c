/* game_engine_allow_pick_up @0x83747188 — ask the active multiplayer game mode whether a unit may pick up an
 * item. Defaults to allowed (1) when there is no game engine or it provides no allow_pick_up handler. */

#include <stdint.h>
#include "headers/game_engine.h"

uint8_t game_engine_allow_pick_up(int unit_index, int item_index)
{
    if ( game_engine && game_engine->allow_pick_up )
        return game_engine->allow_pick_up(unit_index, item_index);
    return 1;
}
