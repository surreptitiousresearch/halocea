/* king_get_team_score @0x8382C938 — getter for a team's King of the Hill score. */

#include "headers/king_globals.h"

int king_get_team_score(int team_index)
{
    return king_globals.score[team_index];
}
