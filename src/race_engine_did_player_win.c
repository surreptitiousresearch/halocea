/* race_engine_did_player_win @0x8382B148 — race-variant win check: without teams, defers to the default
 * per-player win check. With teams, asks race_team_can_win_game for both teams; if they disagree, the
 * player's own team's answer decides; if they agree and both teams can still win, defer to the default check;
 * if neither team can win, the race is over with no winner for this player (-1).
 *
 * DEVIATION: the decompiler modeled the two teams' can-win results as adjacent stack scalars (v5 at
 * [sp+50h], v6 at [sp+51h]) indexed via `*(&v5 + team_index)` — restored as the 2-element array it actually
 * is. Player datum's team_index is read by raw offset (+32; no full player_datum struct in this corpus). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_has_teams(void);
extern uint8_t race_team_can_win_game(int team_index);
extern int game_engine_did_player_win_default(int player_index);

int race_engine_did_player_win(int player_index)
{
    if ( !game_engine_has_teams() )
        return game_engine_did_player_win_default(player_index);

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int team_index = player->team_index;

    uint8_t team_can_win[2];
    team_can_win[0] = race_team_can_win_game(0);
    team_can_win[1] = race_team_can_win_game(1);

    if ( team_can_win[0] != team_can_win[1] )
        return team_can_win[team_index] != 0;

    if ( team_can_win[0] )
        return game_engine_did_player_win_default(player_index);

    return -1;
}
