/* hcex_prof_end @0x823E3C70 — close the HCEX profiling interval opened by hcex_prof_start: accumulate the
 * elapsed performance-counter ticks into hcex_prof.totalTime, convert to milliseconds using the counter
 * frequency, push the sample into the hcex_prof_times ring and reset the accumulators.
 *
 * CAVEAT: the decompiler modeled the 64-bit osGetPerfCounter result as an OVERLAPPED register pair (>>32)
 * and the u64->double conversions as raw __u64tod calls (matching hcex_prof_start's deviation); the disasm
 * accumulates the full 64-bit tick delta. Reconstructed with full 64-bit arithmetic. */

#include "../headers/apProfileTimer.h"
#include "../headers/ds_vector_float8.h"

extern dsVECTOR_float_8 hcex_prof_times;
extern apProfileTimer   hcex_prof;

extern unsigned long long osGetPerfCounter(void);
extern unsigned long long osGetPerfFrequancy(void);
extern "C" double __u64tod(unsigned long long value);
extern float *dsVECTOR_float_8__PushBack(dsVECTOR_float_8 *v, const float *value);  /* dsVECTOR<float,8>::PushBack */

extern "C" float *hcex_prof_end(void)
{
    hcex_prof.totalTime += osGetPerfCounter() - hcex_prof.startTime;

    double elapsed_ticks  = __u64tod(hcex_prof.totalTime);
    double perf_frequency = __u64tod(osGetPerfFrequancy());
    hcex_prof.totalTime = 0;

    float sample_ms = (float)(elapsed_ticks / perf_frequency) * 1000.0f;
    float *result = dsVECTOR_float_8__PushBack(&hcex_prof_times, &sample_ms);
    hcex_prof_times.nElem = 0;
    return result;
}
