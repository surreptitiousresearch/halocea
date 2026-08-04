/* game_engine_man_out @0x83748C4C — true if a player is permanently "manned out" (used up all lives in a
 * lives-limited game variant, or already flagged out, or the current "odd man out" target). Checks, in order:
 * a cached flag (byte +213), whether the lives-limited variant has exhausted the player's lives (dead and
 * score has reached the lives cap), and finally game_engine_player_is_odd_man_out. Shares the player_data
 * salted-lookup with game_engine_player_is_odd_man_out.c; player fields resolved to the DB
 * player_datum struct (unit index, statistics.deaths). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_player_is_odd_man_out(int player_index);

uint8_t game_engine_man_out(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( player->quit_out_of_game )
        return 1;

    if ( global_variant.universal_variant.lives > 0
      && player->unit_index == -1
      && player->statistics.deaths >= global_variant.universal_variant.lives )
    {
        return 1;
    }

    return game_engine_player_is_odd_man_out(player_index) != 0;
}
