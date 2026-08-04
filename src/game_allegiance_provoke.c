/* game_allegiance_provoke @0x83745598 — when one team damages another, refresh the incident-decay timer on
 * the matching (suspicious) allegiance so its accumulated incidents persist longer before decaying. Matches
 * an allegiance in either direction, provided the side being provoked is flagged suspicious. */

#include "headers/game_allegiance.h"
#include <stdint.h>

void game_allegiance_provoke(int16_t aggressor_team_index, int16_t victim_team_index)
{
    game_allegiance *allegiances = game_allegiance_globals->allegiances;
    int allegiance_count = game_allegiance_globals->allegiance_count;

    for ( int i = 0; i < allegiance_count; ++i, ++allegiances )
    {
        int matched = (allegiances->team1_index == aggressor_team_index
                       && allegiances->team2_index == victim_team_index
                       && allegiances->team2_suspicious)
                    || (allegiances->team2_index == aggressor_team_index
                        && allegiances->team1_index == victim_team_index
                        && allegiances->team1_suspicious);
        if ( matched )
        {
            if ( allegiances->current_incidents > 0 )
            {
                uint16_t decay = (uint16_t)allegiances->incident_decay_time;
                if ( decay != 0xFFFF )
                    allegiances->current_incident_decay_time = decay;
            }
            return;
        }
    }
}
