/* oddball_get_team_score @0x838168E8 */
#include "headers/oddball_globals.h"

int oddball_get_team_score(int team_index)
{
    return oddball_globals.team_score[team_index];
}
