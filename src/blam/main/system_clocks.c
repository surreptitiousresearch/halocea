/* system_clocks @ 0x83762C90 — current high-resolution counter.
 * Attested int64_t: the body returns the full 64-bit counter (ld r3, var_10 @83762CBC) and every
 * sampled caller consumes it as a doubleword (std into main_globals / 64-bit mr). The frequency
 * guard is a full 64-bit test (ld + cmpdi @83762CA4..CA8), not a LowPart check. */

#include <stdint.h>
#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

extern int QueryPerformanceFrequency(_LARGE_INTEGER *);
extern int QueryPerformanceCounter(_LARGE_INTEGER *);

int64_t system_clocks(void)
{
    _LARGE_INTEGER now;

    if ( !s_ClockFrequency.QuadPart )
        QueryPerformanceFrequency(&s_ClockFrequency);
    QueryPerformanceCounter(&now);
    return now.QuadPart;
}
