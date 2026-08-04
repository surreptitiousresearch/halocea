/* system_clock_delta_seconds_real @0x83762D20 — convert a high-resolution clock tick delta (a - b) into
 * real seconds by dividing by the clock frequency.
 *
 * Deviation: the decompiler mangled the 64-bit subtract + fcfid conversion (spurious HIDWORD assignment from
 * an unrelated symbol); the disassembly is simply (double)(a - b) / (double)s_ClockFrequency, reproduced. */

/* DEVIATION: dropped <windows.h> — unused here and it collides with the repo's own
 * Win32-boundary headers (_LARGE_INTEGER/tagRECT/HWND__ etc.). s_ClockFrequency and its
 * _LARGE_INTEGER type come from the repo headers via blam_data_globals.h. */
#include <stdint.h>
#include "headers/blam_data_globals.h"

/* return attested double: fdiv (no frsp) live in f1 at blr; caller main_update_time copies f1
 * with fmr (no frsp) @8368A8C8 */
double system_clock_delta_seconds_real(int64_t a, int64_t b)
{
    return (double)(a - b) / (double)s_ClockFrequency.QuadPart;
}
