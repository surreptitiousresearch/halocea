/* game_allegiance_incident @0x83745DE8 — record a hostility incident between an aggressor and victim team.
 * Finds the suspicious allegiance pair (in either order), adds an incident weight that depends on `type`
 * (0 => +1, 1 => +3, 2 => -1), refreshes its decay timer, and if the running incident count reaches the
 * break threshold, breaks the allegiance and reports it (writing *notify_immediately = !requires_communication).
 * Returns 1 if the allegiance was broken, else 0. */

#include <stdint.h>
#include "headers/game_allegiance.h"
#include "headers/allegiance_incident_type.h"
#include "headers/blam_data_globals.h"
extern int _cntlzw(unsigned int);

extern void game_allegiance_broken(game_allegiance *allegiance, uint8_t currently_broken, uint8_t permanently_broken);

uint8_t game_allegiance_incident(int16_t aggressor_team_index, int16_t victim_team_index, int16_t type,
                                 uint8_t *notify_immediately)
{
    game_allegiance *allegiances = game_allegiance_globals->allegiances;
    int allegiance_count = game_allegiance_globals->allegiance_count;
    if ( allegiance_count <= 0 )
        return 0;

    int i = 0;
    while ( 1 )
    {
        int team1_index = allegiances->team1_index;
        if ( team1_index == aggressor_team_index && allegiances->team2_index == victim_team_index
          && allegiances->team2_suspicious )
            break;
        if ( allegiances->team2_index == aggressor_team_index && team1_index == victim_team_index
          && allegiances->team1_suspicious )
            break;
        ++allegiances;
        if ( ++i >= allegiance_count )
            return 0;
    }

    __int16 increment = 0;
    if ( type == _allegiance_incident_accident )
        increment = 1;
    else if ( type == _allegiance_incident_betrayal )
        increment = 3;
    else if ( (unsigned int)type < 3 )   /* _allegiance_incident_forgive */
        increment = -1;

    __int16 new_count = allegiances->current_incidents + increment;
    int decay_time = (unsigned __int16)allegiances->incident_decay_time;
    allegiances->current_incidents = new_count;
    if ( decay_time != 0xFFFF )
        allegiances->current_incident_decay_time = decay_time;

    int threshold = allegiances->incident_threshold;
    if ( threshold != -1 && new_count >= threshold )
    {
        game_allegiance_broken(allegiances, 1u, 0);
        if ( notify_immediately )
            *notify_immediately = (_cntlzw(allegiances->requires_communication) & 0x20) != 0;
        return 1;
    }
    return 0;
}
