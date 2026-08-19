/* sound_get_music_gain @0x83715018 — return the sound manager's current music gain.
 *
 * DEVIATION: the decompiler modeled the float return through a double (soft-float ABI artifact); the value
 * is simply the float music_gain. */

#include "headers/sound_manager_globals.h"

float sound_get_music_gain(void)
{
    return sound_manager_globals.music_gain;
}
