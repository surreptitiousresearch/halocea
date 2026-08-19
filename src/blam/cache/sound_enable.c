/* sound_enable @ 0x83715358
   Enables or disables the sound manager. */

#include <stdint.h>

#include "headers/sound_manager_globals.h"

void sound_enable(uint8_t enabled)
{
    sound_manager_globals.active = enabled;
}
