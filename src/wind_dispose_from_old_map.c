/* wind_dispose_from_old_map @0x837C1A20 — per-map teardown: mark the wind subsystem
 * uninitialized. */

#include "headers/wind_globals.h"

void wind_dispose_from_old_map(void)
{
    wind_globals.initialized = 0;
}
