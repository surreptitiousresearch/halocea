/* system_clock_delta_microseconds @0x83762D80 */
#include <stdint.h>
#include "headers/blam_data_globals.h"

/* DEVIATION: decompiler "local variable allocation has failed" garbage; reconstructed from disasm
 * (subf/mulld/divd/clrlwi pattern, same family as system_clock_delta_milliseconds/_seconds). */
unsigned int system_clock_delta_microseconds(int64_t a, int64_t b)
{
    return (unsigned int)((a - b) * 1000000 / s_ClockFrequency.QuadPart);
}
