/* game_allegiance_update @ 0x837458D8 — tick down each AI team allegiance's incident decay. When an
 * incident's decay timer expires, one incident is forgiven; if that was the last incident the allegiance
 * is restored (game_allegiance_broken with break=false), otherwise the decay timer is rearmed for the
 * next incident. */

#include <stdint.h>
#include "headers/game_allegiance.h"

extern void game_allegiance_broken(game_allegiance *allegiance, uint8_t currently_broken, uint8_t permanently_broken);

void game_allegiance_update(void)
{
    game_allegiance *allegiances = game_allegiance_globals->allegiances;
    int i;

    for ( i = 0; i < game_allegiance_globals->allegiance_count; i++ )
    {
        game_allegiance *allegiance = &allegiances[i];
        if ( allegiance->current_incident_decay_time > 0 )
        {
            __int16 decay = allegiance->current_incident_decay_time - 1;
            allegiance->current_incident_decay_time = decay;
            if ( decay == 0 )
            {
                __int16 remaining = allegiance->current_incidents;
                allegiance->current_incidents = remaining - 1;
                if ( remaining == 1 )
                    game_allegiance_broken(allegiance, 0, 0);   /* last incident forgiven: restore */
                else
                    allegiance->current_incident_decay_time = allegiance->incident_decay_time;
            }
        }
    }
}
