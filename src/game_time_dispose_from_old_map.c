/* game_time_dispose_from_old_map @ 0x8369A7A8 — deactivate game time */
#include "headers/game_time_globals_struct.h"
#include "headers/blam_data_globals.h"
void game_time_dispose_from_old_map(void)
{
    if ( game_time_globals )
    {
        game_time_globals->initialized = 0;
        game_time_globals->active = 0;
    }
}
