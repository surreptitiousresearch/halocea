/* main_stop_time @0x8368AB28 — halt the game clock (clear the time scale and the restart request). */

#include "headers/main_globals.h"

void main_stop_time(void)
{
    main_globals.halt_time_scale = 0;
    main_globals.restart_time = 0;
}
