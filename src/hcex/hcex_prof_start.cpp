/* hcex_prof_start @0x823D12F0 — begin an HCEX profiling interval: clear the accumulated sample vector
 * and latch the current performance counter as the interval start. hcex_prof / hcex_prof_times are
 * ws-engine profiling objects. */

#include "../headers/apProfileTimer.h"
#include "../headers/ds_vector_float8.h"

extern dsVECTOR_float_8 hcex_prof_times;
extern apProfileTimer   hcex_prof;

extern unsigned long long osGetPerfCounter(void);

extern "C" void hcex_prof_start(void)
{
    hcex_prof_times.nElem = 0;
    /* DEVIATION: the decompiler modeled the 64-bit osGetPerfCounter result as an OVERLAPPED register
     * pair and stored (result+4); the disasm (std r3) stores the full 64-bit counter unshifted. */
    hcex_prof.startTime = osGetPerfCounter();
}
