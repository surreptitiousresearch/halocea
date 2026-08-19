/* sound_get_effects_gain @0x83715028 — return the sound manager's current effects gain.
 *
 * DEVIATION: the decompiler modeled the float return through a double (soft-float ABI artifact); the value
 * is simply the float effects_gain. */

#include "headers/sound_manager_globals.h"

float sound_get_effects_gain(void)
{
    return sound_manager_globals.effects_gain;
}
