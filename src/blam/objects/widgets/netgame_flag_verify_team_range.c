/* netgame_flag_verify_team_range @0x837493B8 — debug-build validation stub; the release build strips the
 * assertion body, leaving only the netgame_flags scan that the compiler could not prove side-effect-free. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


void netgame_flag_verify_team_range(int16_t type, int16_t min, int16_t max, char *string)
{
    int count = global_scenario->netgame_flags.count;
    for ( int16_t i = 0; i < count; ++i )
        ;
}
