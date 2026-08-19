/* player_get_starting_location_count @0x836A8FA8 — number of player starting locations in the scenario. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


int16_t player_get_starting_location_count(void)
{
    return (int16_t)global_scenario->players.count;
}
