/* game_engine_allow_pause @ 0x8374A090 — pausing is only allowed while the game engine is in
 * its active (in-progress) mode, not during the post-game sequence. */

#include <stdint.h>
#include "headers/game_engine_globals.h"

uint8_t game_engine_allow_pause(void)
{
    return game_engine_globals.mode == game_engine_mode_active;
}
