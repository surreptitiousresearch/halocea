/* main_save_map_safe @0x836891A0 — request a checkpoint save at the next safe interval,
 * arming the timeout and resetting the save-check counters. No-op if a non-timeout save is already pending. */

#include "headers/main_globals.h"

void main_save_map_safe(void)
{
    if ( !main_globals.save_map || main_globals.save_map_timeout )
    {
        main_globals.save_map = 1;
        main_globals.save_map_safely = 1;
        main_globals.save_map_timeout = 1;
        main_globals.ticks_until_next_save_check = 0;
        main_globals.ticks_unable_to_save = 0;
        main_globals.safe_intervals = 0;
    }
}
