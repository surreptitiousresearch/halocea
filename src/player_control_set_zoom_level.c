/* player_control_set_zoom_level @0x836DEFC8 — request a zoom level for a local player's controlled unit. */

#include <stdint.h>
#include "headers/player_control_globals.h"

void player_control_set_zoom_level(int16_t local_player_index, int16_t zoom_level)
{
    if ( local_player_index != -1 )
        player_control_globals->players[local_player_index].desired_zoom_level = zoom_level;
}
