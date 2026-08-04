/* player_ui_get_single_player_local_player_from_controller @0x83698888 — map a controller index to its
 * single-player local-player slot (0 or 1), or -1 if no slot owns that controller. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

int16_t player_ui_get_single_player_local_player_from_controller(int16_t controller_index)
{
    int16_t local_player_index = 0;
    while ( player_ui_globals.single_player_controller_index[local_player_index] != controller_index )
    {
        if ( ++local_player_index >= 2 )
            return -1;
    }
    return local_player_index;
}
