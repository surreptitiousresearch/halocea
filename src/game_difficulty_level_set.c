/* game_difficulty_level_set @0x836882A8 — set the current game difficulty in the global game options. */

#include <stdint.h>
#include "headers/game_globals.h"

void game_difficulty_level_set(int16_t difficulty)
{
    game_globals->options.difficulty = difficulty;
}
