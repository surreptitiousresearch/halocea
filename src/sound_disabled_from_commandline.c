/* sound_disabled_from_commandline @0x83715378 */
#include "headers/sound_manager_globals.h"

int sound_disabled_from_commandline(void)
{
    return sound_manager_globals.disable_from_commandline;
}
