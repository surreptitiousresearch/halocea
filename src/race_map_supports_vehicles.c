/* race_map_supports_vehicles @0x8382A000 — true if the scenario has at least one race-vehicle-type
 * netgame flag placed. */

#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/blam_data_globals.h"


int race_map_supports_vehicles(void)
{
    int count = global_scenario->netgame_flags.count;
    scenario_netgame_flag *flag = (scenario_netgame_flag *)global_scenario->netgame_flags.address;

    for (int i = 0; i < count; i++, flag++)
    {
        if (flag->type == _netgame_flag_race_vehicle)
            return 1;
    }
    return 0;
}
