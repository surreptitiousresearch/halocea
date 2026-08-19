/* actor_stimulus_surprise @0x837D3D58 — register a surprise stimulus on an actor if it outranks the current
 * one (+750). Records the new level, the surprising prop (+756) and, when given, the surprise direction vector
 * (+764, with presence flag +760). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_surprise(int actor_index, int16_t surprise_level, int prop_index,
                             const real_vector3d *surprise_vector)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( surprise_level > actor->stimuli.surprise_level )
    {
        actor->stimuli.surprise_level = surprise_level;
        actor->stimuli.surprise_prop_index = prop_index;
        if ( surprise_vector )
        {
            actor->stimuli.surprise_has_vector = 1;
            actor->stimuli.surprise_vector = *surprise_vector;
        }
        else
        {
            actor->stimuli.surprise_has_vector = 0;
        }
    }
}
