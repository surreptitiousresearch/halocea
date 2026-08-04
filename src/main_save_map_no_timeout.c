/* main_save_map_no_timeout @0x8368B048 — request a safe checkpoint save with no give-up timeout. If a
 * timed safe-save is already pending, just clear its timeout flag; otherwise start a fresh untimed save. */

#include "headers/main_globals.h"

void main_save_map_no_timeout(void)
{
    if ( main_globals.save_map && !main_globals.save_map_timeout )
    {
        main_globals.save_map_timeout = 0;
    }
    else
    {
        main_globals.save_map = 1;
        main_globals.save_map_safely = 1;
        main_globals.ticks_until_next_save_check = 0;
        main_globals.ticks_unable_to_save = 0;
        main_globals.safe_intervals = 0;
        main_globals.save_map_timeout = 0;
    }
}
