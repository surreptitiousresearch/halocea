/* netgame_flag_verify_no_team_duplicates @0x83749348 — genuinely a no-op in this build: the inner loop that
 * would compare each subsequent netgame flag against the outer one never does anything (empty body) before
 * falling through, so no duplicate is ever flagged or acted on. Confirmed via disasm_range(0x83749348,
 * 0x837493C0), which matches the decompiler's empty `for ( j = i+1; j<count; j++ ) ;` exactly — reproduced
 * faithfully rather than "fixed". */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"

void netgame_flag_verify_no_team_duplicates(int16_t type, char *string)
{
    int count = global_scenario->netgame_flags.count;

    if ( count > 0 )
    {
        scenario_netgame_flag *flags = (scenario_netgame_flag *)global_scenario->netgame_flags.address;

        for ( int i = 0; i < count; ++i )
        {
            if ( type == flags[i].type )
            {
                for ( int j = i + 1; j < count; ++j )
                    ;
            }
        }
    }
}
