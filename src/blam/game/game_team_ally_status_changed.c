/* game_team_ally_status_changed @0x83745448 — look up the allegiance record between two teams (in either order)
 * and return its status_changed flag; 0 if the two teams have no allegiance record. */

#include "headers/game_allegiance.h"
#include <stdint.h>

/* attested uint8_t: returns lbz status_changed byte; sole caller stb-stores r3 */
uint8_t game_team_ally_status_changed(int16_t our_team_index, int16_t other_team_index)
{
    game_allegiance *allegiances = game_allegiance_globals->allegiances;
    int allegiance_count = game_allegiance_globals->allegiance_count;
    if ( allegiance_count > 0 )
    {
        for ( int i = 0; ; ++allegiances )
        {
            short team1_index = allegiances->team1_index;
            if ( team1_index == our_team_index && allegiances->team2_index == other_team_index )
                return allegiances->status_changed;
            if ( allegiances->team2_index == our_team_index && team1_index == other_team_index )
                return allegiances->status_changed;
            if ( ++i >= allegiance_count )
                break;
        }
    }
    return 0;
}
