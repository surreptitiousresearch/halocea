/* players_unzoom_all @0x836DEEE8 — clear the desired zoom level (-1) for both local player-control records. */

#include "headers/player_control_globals.h"

void players_unzoom_all(void)
{
    for ( int i = 0; i < 2; i = (short)(i + 1) )
        player_control_globals->players[i].desired_zoom_level = -1;
}
