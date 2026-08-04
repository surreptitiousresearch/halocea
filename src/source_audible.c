/* source_audible @ 0x83717228 — decide which listener (if any) hears a sound source and how it is
 * obstructed. Unspatialized sources are always audible (returns 0). For world-spatialized sources
 * the nearest valid listener within range is chosen, its obstruction computed, and its index
 * returned; -1 if out of range or fully occluded. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/sound_manager_globals.h"
#include "headers/spatialization_mode.h"

extern float source_distance_squared(int16_t listener_index, const sound_source *source);
extern void compute_sound_obstruction(int16_t local_player_index, sound_source *source, float distance);

int16_t source_audible(sound_source *source, float maximum_distance)
{
    int nearest = -1;

    if ( !source->spatialization_mode )
        return 0;

    if ( source->spatialization_mode == _sound_spatialization_mode_relative )
    {
        if ( source_distance_squared(-1, source) < maximum_distance )
            return 0;
    }
    else
    {
        float best = 3.4028235e38f;
        __int16 i;
        for ( i = 0; i < 2; ++i )
        {
            if ( sound_manager_globals.listeners[i].valid )
            {
                float d = source_distance_squared(i, source);
                if ( d < best )
                {
                    nearest = i;
                    best = d;
                }
            }
        }
        if ( (__int16)nearest != -1 )
            compute_sound_obstruction(nearest, source, __builtin_sqrtf(best));
        if ( best > (maximum_distance * maximum_distance) || source->occlusion == 1.0f )
            return -1;
    }
    return (__int16)nearest;
}
