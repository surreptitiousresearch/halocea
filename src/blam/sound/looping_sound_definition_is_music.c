/* looping_sound_definition_is_music @0x83714638 — true if any of a looping_sound_definition's tracks
 * references a sound tag whose class_index is _sound_class_music (32). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/sound_definition.h"
#include "headers/sound_class.h"

uint8_t looping_sound_definition_is_music(int definition_index)
{
    looping_sound_definition *definition = TAG_GET(looping_sound_definition, definition_index);
    int track_count = definition->tracks.count;

    if ( track_count <= 0 )
        return 0;

    looping_sound_track *tracks = (looping_sound_track *)definition->tracks.address;

    for ( int i = 0; i < track_count; ++i )
    {
        int sound_index = tracks[i].loop_sound.index;

        if ( sound_index != -1 && TAG_GET(sound_definition, sound_index)->class_index == _sound_class_music )
            return 1;
    }

    return 0;
}
