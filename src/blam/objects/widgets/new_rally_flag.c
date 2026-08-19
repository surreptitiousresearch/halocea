/* new_rally_flag @0x8382B5E0 — pick the next rally-race flag: count the candidate flags in the
 * race_globals lap-completed bit mask (one bit per rally flag placed in the map), exclude the current flag
 * from the draw, roll a game-state random index over the survivors, and walk the scenario netgame flags
 * (148-byte elements; type word +16 == 3 = rally flag, its slot index in the word at +18) until the rolled
 * survivor is reached. Returns -1 when no candidate exists. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/race_globals.h"
#include "headers/blam_data_globals.h"


extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int new_rally_flag(int old_flag)
{
    int16_t candidate_count = 0;
    for ( int bit = 0; bit < 32; bit++ )
    {
        if ( (race_globals.lap_completed_value & (1 << bit)) != 0 )
            ++candidate_count;
    }
    if ( old_flag != -1 )
        --candidate_count;

    int16_t pick = seed_random_range(get_global_random_seed_address(), 0, candidate_count);

    scenario *scnr = global_scenario;
    scenario_netgame_flag *netgame_flag = (scenario_netgame_flag *)scnr->netgame_flags.address;
    for ( int i = 0; i < scnr->netgame_flags.count; i++, netgame_flag++ )
    {
        if ( netgame_flag->type != _netgame_flag_race_track )
            continue;
        int flag_index = netgame_flag->team_index; /* rally flag slot index reuses the team_index field */
        if ( flag_index == old_flag )
            continue;
        if ( !pick )
            return flag_index;
        --pick;
    }
    return -1;
}
