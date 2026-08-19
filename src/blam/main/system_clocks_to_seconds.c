/* system_clocks_to_seconds @0x83762CD0 — divides a raw clock count by the cached clock frequency (see
 * system_clocks.c for the same global). */

#include <stdint.h>
#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"


unsigned int system_clocks_to_seconds(int64_t clocks)
{
    return (unsigned int)(clocks / s_ClockFrequency.QuadPart);
}
