/* ctf_get_team_score @0x83806D60 */
#include "headers/ctf_globals.h"

int ctf_get_team_score(int team_index)
{
    return ctf_globals.score[team_index];
}
