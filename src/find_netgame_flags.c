/* find_netgame_flags @0x83747558 — collect up to `max` scenario netgame-flag indices matching a type/team
 * filter, optionally also gated by 3D distance + separate height-delta from `location`; writes indices into
 * `flags` and returns the count found. DEVIATION: the decompiler's own parameter list is scrambled — it
 * displays 9 parameters (with phantom a8/a9) and its named `max`/`flags` locals are actually bound to the
 * real `type`/`team` argument values, while the genuine max-count and output-array arguments show up as the
 * phantom a8/a9. Recovered the true register mapping from disasm (r6=type, r7=team, r8=max, r9=flags),
 * which matches the DB's own 7-argument prototype exactly. */

#include "headers/scenario.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


#include <stdint.h>

extern double __fabs(double);

int find_netgame_flags(const real_point3d *location, float distance, float height_delta, int16_t type, int16_t team,
                        int max, int *flags)
{
    int found_count = 0;
    scenario_netgame_flag *netgame_flags = (scenario_netgame_flag *)global_scenario->netgame_flags.address;
    float distance_squared = distance * distance;

    for ( int16_t i = 0; i < global_scenario->netgame_flags.count; i++ )
    {
        scenario_netgame_flag *flag = &netgame_flags[i];

        if ( (type != -1 && type != flag->type) || (team != -1 && team != flag->team_index) )
            continue;

        if ( location )
        {
            if ( distance >= 0.0f )
            {
                float dx = location->n[0] - flag->position.n[0];
                float dy = location->n[1] - flag->position.n[1];
                float dz = location->n[2] - flag->position.n[2];
                if ( dx * dx + dy * dy + dz * dz > distance_squared )
                    continue;
            }
            if ( height_delta > 0.0f && __fabs(flag->position.n[2] - location->n[2]) > height_delta )
                continue;
        }

        if ( found_count < max )
            flags[found_count++] = i;
    }

    return found_count;
}
