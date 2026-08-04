/* race_flags_make_unique @0x8382A18C — ensure every "race flag" (netgame_flags[i].type == 3) has a unique
 * flag_index in [0,32): for each race flag whose bit is already taken, hunt for the lowest free bit and
 * store that as the flag's new flag_index — but note the tracking bitmask itself gets OR'd with the
 * *original* (colliding) bit, not the newly assigned one (confirmed via disasm at 0x8382A1E8-0x8382A218:
 * `or r9,r10,r9` uses r10, which still holds `1 << original_flag_index`, never reassigned to the found free
 * bit). This looks like a bug in the original game code, but it's exactly what the compiled binary does, so
 * it's reproduced faithfully rather than "corrected". Returns the final bitmask. */

#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"

int race_flags_make_unique(void)
{
    unsigned int used_flags = 0;

    for ( int i = 0; i < global_scenario->netgame_flags.count; ++i )
    {
        scenario_netgame_flag *netgame_flag =
            &((scenario_netgame_flag *)global_scenario->netgame_flags.address)[i];

        if ( (unsigned __int16)netgame_flag->type != _netgame_flag_race_track )
            continue;

        __int16 flag_index = netgame_flag->team_index;
        if ( flag_index < 0 || flag_index >= 0x20 )
            continue;

        unsigned int flag_bit = 1u << flag_index;
        if ( (flag_bit & used_flags) == 0 )
        {
            used_flags |= flag_bit;
            continue;
        }

        int free_bit = 0;
        while ( 1 )
        {
            if ( ((1u << free_bit) & used_flags) == 0 )
                break;
            if ( ++free_bit >= 0x20 )
            {
                netgame_flag->team_index = (__int16)free_bit;
                goto next;
            }
        }

        used_flags |= flag_bit;
        netgame_flag->team_index = (__int16)free_bit;

next:;
    }

    return used_flags;
}
