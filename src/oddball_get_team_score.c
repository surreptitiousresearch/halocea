#include "headers/oddball_globals.h"

int oddball_get_team_score(int team_index)
{
    return oddball_globals.team_score[team_index];
}
