/* sound_is_active @0x83715130 */
#include "headers/sound_manager_globals.h"

int sound_is_active(void)
{
    if ( !sound_manager_globals.initialized )
        return 0;
    if ( !sound_manager_globals.active )
        return 0;
    if ( sound_manager_globals.disable_from_commandline )
        return 0;
    return 1;
}
