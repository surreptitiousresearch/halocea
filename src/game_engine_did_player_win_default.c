/* game_engine_did_player_win_default @0x8374E5B8 — default "did this player win" test used by game variants
 * that don't override it. Team games: -1 (undecided) unless only one team remains alive or the two teams'
 * scores differ, in which case compares the player's team to whichever team is winning. Non-team games:
 * based on `game_engine_get_player_place`'s `place` field (0 = first place = win); place == INT_MIN is treated
 * as a distinct "no result" sentinel, matching the DB's bit-masked comparison exactly.
 *
 * DEVIATION — the DB's own decompile copies `game_engine_get_player_place`'s entire 28-byte result through a
 * confusing 7-DWORD manual loop into a separate local, when only the last field (`place`) is ever read
 * afterward; simplified to a direct `.place` access (behaviorally identical — the intermediate buffer has no
 * other observer). */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/player_datum.h"
#include "headers/statistic_buffer.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern uint8_t multiple_teams_alive(void);
extern uint8_t team_has_players(int team_index);
extern statistic_buffer * game_engine_get_player_place(statistic_buffer *result, int player_index);

int game_engine_did_player_win_default(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if (global_variant.universal_variant.teams)
    {
        int team0_score = game_engine->get_team_score(0);
        int team1_score = game_engine->get_team_score(1);

        if (!multiple_teams_alive())
            return (team_has_players(0) == 0) == player->team_index;

        if (team0_score != team1_score)
        {
            int winning_team = (team1_score >> 31) + (team1_score >= team0_score) + (team0_score >> 31);
            return winning_team == player->team_index;
        }

        return -1;
    }

    statistic_buffer player_place;
    game_engine_get_player_place(&player_place, player_index);

    int place = player_place.place;
    if (place < 0 && (place & 0x7FFFFFFF) == 0)
        return -1;

    return (place & 0x7FFFFFFF) == 0;
}
