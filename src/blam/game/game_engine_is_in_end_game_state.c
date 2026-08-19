/* game_engine_is_in_end_game_state @0x83746208 — true when the game engine is in any post-game phase
 * (mode other than active). */

#include <stdint.h>
#include "headers/game_engine_globals.h"

uint8_t game_engine_is_in_end_game_state(void)
{
    return game_engine_globals.mode != game_engine_mode_active;
}
