/* main_reset_time @0x8368A838 — re-sample the system clock and reset the per-frame timing fields.
 * (Body identical to main_initialize_time.)
 *
 * Deviation: the __int128 / +4-offset register juggling is PPC ABI noise around the 64-bit clock value.
 * Deviation: attested void — r3 at the blr is only the threaded system_clocks_to_milliseconds result
 * (bl + epilogue, no explicit r3 computation) and the binary has no callers that consume it. */

#include <stdint.h>
#include "headers/main_globals.h"

extern int64_t system_clocks(void);
extern uint32_t system_clocks_to_milliseconds(int64_t clocks);

void main_reset_time(void)
{
    int64_t clocks = system_clocks();
    main_globals.last_time_clocks = clocks;
    main_globals.last_render_clocks = clocks;
    main_globals.last_time_msec = system_clocks_to_milliseconds(clocks);
}
