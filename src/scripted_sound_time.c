/* scripted_sound_time @0x83713B60 — return the remaining play time (in ticks) of a scripted sound, derived
 * from the sound tag's stored end-time (tag data +0x90), or 0 if the sound is not playing.
 *
 * DEVIATION: the binary computes max(0, end_time - now) with a branchless saturate idiom; reproduced as the
 * clamped subtraction. */

#include "headers/global_tag_instances.h"
#include "headers/sound_definition.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

int scripted_sound_time(int definition_index)
{
    if ( definition_index == -1 )
        return 0;
    sound_definition *tag_data = TAG_GET(sound_definition, definition_index);
    int end_time = tag_data->runtime_scripting_time;
    if ( end_time == -1 )
        return 0;
    int remaining = end_time - game_time_get();
    return remaining > 0 ? remaining : 0;
}
