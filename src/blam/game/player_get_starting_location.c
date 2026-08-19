/* player_get_starting_location @0x836A8FC0 — return the scenario_player at the given starting-location index,
 * or null if the index is out of range. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_player.h"
#include "headers/blam_data_globals.h"


scenario_player * player_get_starting_location(int16_t location_index)
{
    if ( location_index >= 0 && location_index < global_scenario->players.count )
        return &((scenario_player *)global_scenario->players.address)[location_index];
    return 0;
}
