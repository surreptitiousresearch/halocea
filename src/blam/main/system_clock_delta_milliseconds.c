/* system_clock_delta_milliseconds @0x83762D60 */
#include <stdint.h>
#include "headers/blam_data_globals.h"

/* DEVIATION: decompiler output garbled; reconstructed from disasm (subf/mulli/divd/clrlwi pattern) */
unsigned int system_clock_delta_milliseconds(int a, int b)
{
    return (unsigned int)((int64_t)(a - b) * 1000 / s_ClockFrequency.QuadPart);
}
