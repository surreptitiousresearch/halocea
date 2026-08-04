/* game_time_initialized @0x8369A778 — true once the game-time subsystem globals exist and have been
 * marked initialized. */

#include "headers/blam_data_globals.h"

int game_time_initialized(void)
{
    if ( !game_time_globals )
        return 0;
    if ( !game_time_globals->initialized )
        return 0;
    return 1;
}
