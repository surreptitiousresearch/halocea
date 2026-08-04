/* hcex_prof_update @0x823E3BD0 — "lap" the HCEX profiling interval: fold the elapsed performance-counter
 * ticks since the last start into hcex_prof.totalTime, convert that to milliseconds via the counter
 * frequency and push it as a sample into hcex_prof_times, then reset the accumulator and re-latch startTime
 * so the next interval measures from here. Unlike hcex_prof_end it does NOT clear the sample ring (nElem is
 * left intact) — successive updates accumulate samples.
 *
 * CAVEAT: the decompiler mangled this into OVERLAPPED 64-bit register pairs (a >>32 on the counter, a bogus
 * 0x82000000_00000000 store into totalTime, and a (result+4) start-time store). The disassembly (std of the
 * full 64-bit values) shows plain 64-bit arithmetic; reconstructed accordingly, mirroring hcex_prof_end. */

#include "../headers/apProfileTimer.h"
#include "../headers/ds_vector_float8.h"

extern dsVECTOR_float_8 hcex_prof_times;
extern apProfileTimer   hcex_prof;

extern unsigned long long osGetPerfCounter(void);
extern unsigned long long osGetPerfFrequancy(void);
extern double __u64tod(unsigned long long value);
extern float *dsVECTOR_float_8__PushBack(dsVECTOR_float_8 *v, const float *value);  /* dsVECTOR<float,8>::PushBack */

extern "C" void hcex_prof_update(void)
{
    hcex_prof.totalTime += osGetPerfCounter() - hcex_prof.startTime;

    double elapsed_ticks  = __u64tod(hcex_prof.totalTime);
    double perf_frequency = __u64tod(osGetPerfFrequancy());
    hcex_prof.totalTime = 0;

    float sample_ms = (float)(elapsed_ticks / perf_frequency) * 1000.0f;
    dsVECTOR_float_8__PushBack(&hcex_prof_times, &sample_ms);

    hcex_prof.startTime = osGetPerfCounter();
}
