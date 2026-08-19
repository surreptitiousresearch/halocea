/* game_allegiance_notify_change @0x83745658 — clear the status-changed flag on the allegiance record matching
 * the given team pair (in either order). */

#include "headers/game_allegiance.h"
#include "headers/blam_data_globals.h"


#include <stdint.h>

void game_allegiance_notify_change(int16_t aggressor_team_index, int16_t victim_team_index)
{
    game_allegiance *allegiances = game_allegiance_globals->allegiances;
    int allegiance_count = game_allegiance_globals->allegiance_count;
    if ( allegiance_count <= 0 )
        return;

    int i = 0;
    while ( 1 )
    {
        int team1_index = allegiances->team1_index;
        if ( team1_index == aggressor_team_index && allegiances->team2_index == victim_team_index )
            break;
        if ( allegiances->team2_index == aggressor_team_index && team1_index == victim_team_index )
            break;
        ++allegiances;
        if ( ++i >= allegiance_count )
            return;
    }
    allegiances->status_changed = 0;
}
