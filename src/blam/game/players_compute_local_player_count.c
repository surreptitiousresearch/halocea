/* players_compute_local_player_count @0x836A9680 — count the assigned local-player slots (players_globals
 * local_player_indices[2]; -1 means unassigned). */

#include "headers/players_globals.h"

int players_compute_local_player_count(void)
{
    int count = 0;
    for ( int i = 0; i < 2; i = (short)(i + 1) )
    {
        if ( players_globals->local_player_indices[i] != -1 )
            count = (short)(count + 1);
    }
    return count;
}
