/* in_scoring_range @0x83806508 — is `location` within `distance` of the given team's CTF flag?
 *
 * DEVIATION: the previous reading invented a 4th parameter because it assumed `float distance` does
 * not consume a GPR slot. It does — a float argument occupies its parameter slot and lives in the
 * next free FPR — so r5 IS team_index (slot 2), not a slot-3 argument. `slwi r9, r5, 2` at
 * 0x8380651C indexes ctf_globals.flags[team_index] directly, and the DB's 3-parameter prototype
 * is correct. */

#include "headers/real_point3d.h"
#include "headers/ctf_globals.h"
#include "headers/scenario_netgame_flag.h"

BOOL in_scoring_range(real_point3d *location, float distance, int team_index)
{
    if ( !location )
        return 0;

    scenario_netgame_flag *flag = ctf_globals.flags[team_index];
    if ( !flag )
        return 0;

    float dx = flag->position.n[0] - location->n[0];
    float dy = flag->position.n[1] - location->n[1];
    float dz = flag->position.n[2] - location->n[2];

    return dx * dx + dy * dy + dz * dz < distance * distance;
}
