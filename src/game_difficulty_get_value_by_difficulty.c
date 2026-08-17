/* game_difficulty_get_value_by_difficulty @0x837B66F8 */
/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_globals_definition.h"
#include "headers/game_globals_difficulty_information.h"
#include "headers/game_difficulty.h"


float game_difficulty_get_value_by_difficulty(int16_t value_type, int16_t difficulty)
{
    if ( !global_game_globals || !global_game_globals->difficulty_information.count )
        return 1.0f;

    game_globals_difficulty_information *difficulty_table = (game_globals_difficulty_information *)global_game_globals->difficulty_information.address;
    if ( !difficulty_table )
        return 1.0f;

    int16_t clamped_difficulty = difficulty;
    if ( difficulty < game_difficulty_level_easy )
        clamped_difficulty = 0;
    else if ( difficulty > game_difficulty_level_impossible )
        clamped_difficulty = 3;

    return difficulty_table->game_difficulty_values[value_type][clamped_difficulty];
}
