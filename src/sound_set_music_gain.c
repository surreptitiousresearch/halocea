/* sound_set_music_gain @0x8371A654 — change the global music gain and re-apply it to all channels.
 * No-op when unchanged. Muting (was audible, now <=0) disables the "music" sound class and zeroes the gain.
 * Un-muting from silence (was 0, now >0) re-enables the class and clamps gains >=1.0 down to 1.0; otherwise
 * the gain is stored verbatim.
 * DEVIATION: update_channels is a no-arg function (DB arg_count 0); the caller leaves r3 stale. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

extern void debug_sound_classes_enable(const char *substring, uint8_t enabled);
extern void update_channels(void);

void sound_set_music_gain(float gain)
{
    float music_gain = sound_manager_globals.music_gain;
    if ( gain != music_gain )
    {
        if ( music_gain <= 0.0f || gain > 0.0f )
        {
            if ( music_gain == 0.0f && gain > 0.0f && (debug_sound_classes_enable("music", 1), gain >= 1.0f) )
                sound_manager_globals.music_gain = 1.0f;
            else
                sound_manager_globals.music_gain = gain;
        }
        else
        {
            debug_sound_classes_enable("music", 0);
            sound_manager_globals.music_gain = 0.0f;
        }
        update_channels();
    }
}
