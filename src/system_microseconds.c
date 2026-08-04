/* system_microseconds @0x83762C30 — DEVIATION: the decompiler flagged "local variable allocation has
 * failed" and rendered the 64-bit result as a broken HIDWORD/LODWORD split. disasm_range(0x83762C30,
 * 0x83762C90) shows plain PPC64 64-bit arithmetic (`ld`/`mulld`/`divdu`, single r3 return) — this is just
 * system_clocks.c's counter/frequency pattern (same s_ClockFrequency global, confirmed by that function
 * starting exactly at this function's end address 0x83762C90) scaled to microseconds instead of truncated
 * to the low 32 bits. */

#include <stdint.h>

#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

extern int QueryPerformanceFrequency(_LARGE_INTEGER *);
extern int QueryPerformanceCounter(_LARGE_INTEGER *);

uint64_t system_microseconds(void)
{
    _LARGE_INTEGER now;

    if ( !s_ClockFrequency.u.LowPart )
        QueryPerformanceFrequency(&s_ClockFrequency);
    QueryPerformanceCounter(&now);
    return (uint64_t)now.QuadPart * 1000000 / s_ClockFrequency.QuadPart;
}
