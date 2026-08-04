/* players_dispose @ 0x836A78A8 — drop player/team/globals pointers */
#include "headers/data_array.h"
#include "headers/players_global_data.h"
#include "headers/blam_data_globals.h"
void players_dispose(void)
{
    if ( player_data )     player_data = 0;
    if ( team_data )       team_data = 0;
    if ( players_globals ) players_globals = 0;
}
