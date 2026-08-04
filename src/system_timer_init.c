/* system_timer_init @0x83762B68 — tail-call forwarder (`lis/addi &s_ClockFrequency; b
 * QueryPerformanceFrequency`; confirmed via disasm, not a bare blr). Primes the cached clock frequency used
 * by system_clocks. */

#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

extern int QueryPerformanceFrequency(_LARGE_INTEGER *);

void system_timer_init(void)
{
    QueryPerformanceFrequency(&s_ClockFrequency);
}
