/* in_scoring_range @0x83806508 — DEVIATION: the DB's own funcs.prototype lists only 3 params (location,
 * distance, team_index), but disasm_range(0x83806508,0x83806568) shows `slwi r9, r5, 2` indexing
 * ctf_globals.flags[] — r5 is the 4th argument register (after r3=location, f1=distance, r4=team_index),
 * meaning team_index (r4) is real but genuinely unread, and a real 4th parameter (r5) is what selects the
 * flag. Reproduced with the decompiler's original 4-param reading. */

#include "headers/real_point3d.h"
#include "headers/ctf_globals.h"
#include "headers/scenario_netgame_flag.h"

BOOL in_scoring_range(real_point3d *location, float distance, int team_index, int flag_team_index)
{
    if ( !location )
        return 0;

    scenario_netgame_flag *flag = ctf_globals.flags[flag_team_index];
    if ( !flag )
        return 0;

    float dx = flag->position.n[0] - location->n[0];
    float dy = flag->position.n[1] - location->n[1];
    float dz = flag->position.n[2] - location->n[2];

    return dx * dx + dy * dy + dz * dz < distance * distance;
}
