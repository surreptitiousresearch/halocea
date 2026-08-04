/* sound_get_master_gain @0x83715008 — return the sound manager's current master gain.
 *
 * DEVIATION: the decompiler modeled the float return through a double (HIDWORD artifact of the soft-float
 * ABI); the function simply returns the float master_gain. */

#include "headers/sound_manager_globals.h"


float sound_get_master_gain(void)
{
    return sound_manager_globals.master_gain;
}
