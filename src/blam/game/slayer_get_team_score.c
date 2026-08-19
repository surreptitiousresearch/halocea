/* slayer_get_team_score @0x83815790 */
#include "headers/slayer_globals.h"

int slayer_get_team_score(int team_index)
{
    return slayer_globals.team_score[team_index];
}
