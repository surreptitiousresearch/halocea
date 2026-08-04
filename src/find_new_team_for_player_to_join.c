/* find_new_team_for_player_to_join @0x83802BC0 — pick the team (0 or 1) a player should be placed on in a
 * team game: the team with fewer players (excluding the joining player), or on a player-count tie the team
 * with the lower score. Returns 0 when no engine is running or the variant has no teams.
 *
 * DEVIATION 1 (bug class 4, verified numerically against disasm): the two branchless carry-chain forms
 * ((sign/eqv + subfc/adde/addze) at 0x83802C2C and 0x83802C4C) decode to the plain signed comparisons
 * written below.
 * DEVIATION 2 (faithful quirk): on the tied-count path the compiled code calls
 * game_engine_get_team_score(0) and (1) and DISCARDS both results (disasm: r3 immediately overwritten),
 * then compares the scores teamplay_get_team_info already produced. Reproduced verbatim. */

#include <stdint.h>

extern uint8_t game_engine_running(void);
extern uint8_t game_engine_has_teams(void);
extern void teamplay_get_team_info(int *players, int *scores, int skip_player_index);
extern int game_engine_get_team_score(int team_index);

uint8_t find_new_team_for_player_to_join(int player_index)
{
    if ( !game_engine_running() || !game_engine_has_teams() )
        return 0;

    int players[2];
    int scores[2];
    teamplay_get_team_info(players, scores, player_index);

    if ( players[0] != players[1] )
        return players[0] >= players[1];

    game_engine_get_team_score(0);   /* results discarded, as compiled */
    game_engine_get_team_score(1);
    return scores[1] < scores[0];
}
