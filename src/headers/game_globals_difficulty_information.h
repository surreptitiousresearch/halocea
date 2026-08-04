#pragma once
/* game_globals_difficulty_information — the difficulty block of the game_globals ('matg') tag
 * (644 bytes, DB-verified). game_difficulty_values[value_type][difficulty_level] holds the
 * difficulty-scaled tuning table sampled by game_difficulty_get_value. */

typedef struct game_globals_difficulty_information
{
    float        game_difficulty_values[35][4];  /* 0x000 */
    unsigned int unused[21];                     /* 0x230 */
} game_globals_difficulty_information;            /* 0x284 (644 bytes) */
