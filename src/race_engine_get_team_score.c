/* race_engine_get_team_score @0x8382B000 — getter for a team's completed-lap count. */

#include "headers/race_globals.h"

int race_engine_get_team_score(int team_index)
{
    return race_globals.team_laps[team_index];
}
