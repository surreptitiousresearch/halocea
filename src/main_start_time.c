/* main_start_time @0x8368AB48 — request the main loop to restart the game clock. */

#include "headers/main_globals.h"

void main_start_time(void)
{
    main_globals.restart_time = 1;
}
