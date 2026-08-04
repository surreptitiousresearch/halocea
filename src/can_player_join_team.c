#include <stdint.h>
/* can_player_join_team @0x83802C80 — is a player allowed onto `team` (0/1)? Always when the target team is
 * no larger than the other (excluding the joining player); otherwise only when the target team trails the
 * other team's score by at least 20% of it. Teams beyond 1 are refused.
 *
 * DEVIATION (bug class 9): the decompiler's __int64 register puns are the two int→float conversions of the
 * score difference and the other team's score; decoded per disasm. */

extern void teamplay_get_team_info(int *players, int *scores, int skip_player_index);

uint8_t can_player_join_team(int player_index, unsigned int team)
{
    if ( team > 1 )
        return 0;

    int players[2];
    int scores[2];
    teamplay_get_team_info(players, scores, player_index);

    if ( players[team] <= players[1 - team] )
        return 1;

    int score_deficit = scores[1 - team] - scores[team];
    return (float)score_deficit >= (float)scores[1 - team] * 0.2f;
}
