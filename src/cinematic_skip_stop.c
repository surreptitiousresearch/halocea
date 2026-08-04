/* cinematic_skip_stop @0x83690F90 — finish skipping a cinematic: restore the pre-cinematic music gain (if
 * one was saved) and clear the skip-in-progress flag. */

#include "headers/cinematic_globals.h"
#include "headers/blam_data_globals.h"

extern void sound_set_music_gain(float gain);

void cinematic_skip_stop(void)
{
    if ( music_gain_in_profile != -1.0f )
        sound_set_music_gain(music_gain_in_profile);
    music_gain_in_profile = -1.0f;
    cinematic_globals->cinematic_skip_in_progress = 0;
}
