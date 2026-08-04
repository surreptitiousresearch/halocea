/* observer_reconnect_to_structure_bsp @0x8370EBE0 — re-resolve each local player's observer location/cluster
 * from its last known position after a structure BSP swap. */

#include <stdint.h>
#include "headers/observer_globals.h"

extern int local_player_get_player_index(int16_t local_player_index);
extern void scenario_location_from_point(location *location, const real_point3d *point);

int observer_reconnect_to_structure_bsp(void)
{
    __int16 local_player_index;

    for ( local_player_index = 0; local_player_index < 2; ++local_player_index )
    {
        if ( local_player_get_player_index(local_player_index) != -1 )
        {
            scenario_location_from_point(
                &observer_globals.local_players[local_player_index].result.location,
                &observer_globals.local_players[local_player_index].result.position);
        }
    }
    return local_player_index;
}
