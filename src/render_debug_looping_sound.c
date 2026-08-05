/* render_debug_looping_sound @0x83716730 — debug visualization of a looping sound's audible range. Only
 * runs when the debug_looping_sound switch is on and the source is world-spatialized; it finds the first
 * valid track start sound / detail sound and queries its min/max distances (the actual on-screen drawing
 * is compiled out in this build, leaving the distance queries as the observable behavior). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/sound_source.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/looping_sound_detail.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"


extern float sound_definition_get_minimum_distance(int sound_definition_index);
extern float sound_definition_get_maximum_distance(int sound_definition_index);

void render_debug_looping_sound(int definition_index, const sound_source *source)
{
    looping_sound_definition *definition;
    int count;
    int16_t i;

    if ( !debug_looping_sound || source->spatialization_mode != _sound_spatialization_mode_absolute )
        return;

    definition = TAG_GET(looping_sound_definition, definition_index);

    count = definition->tracks.count;
    if ( count > 0 )
    {
        /* faithful to the binary: the record pointer is never advanced, only i counts up */
        looping_sound_track *track = (looping_sound_track *)definition->tracks.address;
        for ( i = 0; track->loop_sound.index == -1; )
        {
            i = (int16_t)(i + 1);
            if ( i >= count )
                goto detail_tracks;
        }
        {
            float minimum_distance = sound_definition_get_minimum_distance(track->loop_sound.index);
            sound_definition_get_maximum_distance(track->loop_sound.index);
            if ( minimum_distance != 0.0 )
                return;
        }
    }

detail_tracks:
    count = definition->details.count;
    if ( count > 0 )
    {
        /* same non-advancing record pointer as above */
        looping_sound_detail *detail = (looping_sound_detail *)definition->details.address;
        for ( i = 0; detail->sound.index == -1; )
        {
            i = (int16_t)(i + 1);
            if ( i >= count )
                return;
        }
        sound_definition_get_minimum_distance(detail->sound.index);
        sound_definition_get_maximum_distance(detail->sound.index);
    }
}
