/* main_get_mseconds @0x83688A58 — return the current frame's millisecond timestamp: the recorded value
 * during a timedemo, otherwise the live system clock. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

extern uint32_t system_milliseconds(void);

uint32_t main_get_mseconds(void)
{
    if ( timeDemo )
        return main_globals.last_time_msec;
    return system_milliseconds();
}
