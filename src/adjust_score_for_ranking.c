/* adjust_score_for_ranking @0x8374BA50 — biases a player's raw display score into a packed ranking key
 * used by populate_statistic_buffer's combined view. Clamps to a -1000 floor, then shifts into a
 * non-negative range and ORs in two tie-breaker bits: bit 30 if the player still has lives remaining
 * (statistics.deaths < the variant's lives limit), and bit 29 if the player has NOT quit out of the game
 * (so quitters rank below otherwise-equal scores). */

#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


int adjust_score_for_ranking(int player_index, int score)
{
    int ranking_flags = 0;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if (score < -1000)
        score = -1000;
    score += 1000;

    if (player->statistics.deaths < global_variant.universal_variant.lives)
        ranking_flags = 0x40000000;

    if (player->quit_out_of_game)
        return ranking_flags | score;
    return ranking_flags | 0x20000000 | score;
}
