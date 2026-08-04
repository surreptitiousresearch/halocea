/* main_reset_time @0x8368A838 — re-sample the system clock and reset the per-frame timing fields; return
 * the current millisecond timestamp. (Body identical to main_initialize_time.)
 *
 * Deviation: the __int128 / +4-offset register juggling is PPC ABI noise around the 64-bit clock value. */

#include <stdint.h>
#include "headers/main_globals.h"

extern int64_t system_clocks(void);
extern uint32_t system_clocks_to_milliseconds(int64_t clocks);

unsigned int main_reset_time(void)
{
    __int64 clocks = system_clocks();
    main_globals.last_time_clocks = clocks;
    main_globals.last_render_clocks = clocks;
    main_globals.last_time_msec = system_clocks_to_milliseconds(clocks);
    return main_globals.last_time_msec;
}
