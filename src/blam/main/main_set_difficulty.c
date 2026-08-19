#include <stdint.h>
#include "headers/game_difficulty.h"
#include "headers/blam_data_globals.h"
/* main_set_difficulty @0x83688FE0 — set the global game difficulty level, ignoring out-of-range values
 * (valid difficulties are 0..3). */


void main_set_difficulty(int16_t difficulty)
{
    if ( difficulty >= game_difficulty_level_easy && difficulty < number_of_game_difficulty_levels )
        global_difficulty_level = difficulty;
}
