/* scripted_sound_stop @0x83713BD0 — stop a scripted (impulse) sound that was started via scripted_sound_new,
 * using the playing-sound handle stored on the sound tag (tag data +0x94), then clear the handle and the
 * stored end-time (+0x90). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/sound_definition.h"
#include "headers/blam_data_globals.h"

extern void sound_stop_impulse(int sound_index);

void scripted_sound_stop(int definition_index)
{
    if ( definition_index == -1 )
        return;
    sound_definition *tag_data = TAG_GET(sound_definition, definition_index);
    int sound_handle = tag_data->runtime_scripting_sound_index;
    if ( sound_handle != -1 )
    {
        sound_stop_impulse(sound_handle);
        tag_data->runtime_scripting_sound_index = -1;
        tag_data->runtime_scripting_time = -1;
    }
}
