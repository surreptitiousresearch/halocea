/* game_allegiance_get_incidents @0x837454F0 — find the allegiance pair for (our_team_index,
 * other_team_index) in either order and return its current_incidents count, writing its
 * incident_threshold out through `incident_threshold` (or -1 if no such pair exists). */

#include <stdint.h>
#include "headers/game_allegiance.h"

int16_t game_allegiance_get_incidents(int16_t our_team_index, int16_t other_team_index, int16_t *incident_threshold)
{
    int16_t threshold = -1;
    int16_t result = 0;

    game_allegiance *allegiances = game_allegiance_globals->allegiances;
    int allegiance_count = game_allegiance_globals->allegiance_count;

    if ( allegiance_count > 0 )
    {
        int i = 0;
        while ( 1 )
        {
            int16_t team1_index = allegiances->team1_index;
            if ( team1_index == our_team_index && allegiances->team2_index == other_team_index )
                break;
            if ( allegiances->team2_index == our_team_index && team1_index == other_team_index )
                break;
            ++allegiances;
            if ( ++i >= allegiance_count )
                goto not_found;
        }

        result = (uint16_t)allegiances->current_incidents;
        threshold = allegiances->incident_threshold;
    }

not_found:
    if ( incident_threshold )
        *incident_threshold = threshold;
    return result;
}
