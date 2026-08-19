/* wind_initialize_for_new_map @0x837C1FD8 — reset the wind globals for a new map and rebuild the cyclic
 * variance noise table. */

#include <string.h>
#include "headers/wind_globals.h"

extern void wind_variance_initialize(void);

void wind_initialize_for_new_map(void)
{
    memset(&wind_globals, 0, sizeof(wind_globals));
    wind_globals.initialized = 1;
    wind_variance_initialize();
}
