// apProfileTimer::GetResSec @ 0x823CDEA0
// Convert accumulated ticks to seconds and reset the accumulator.
#include "apProfileTimer.h"

extern "C" unsigned __int64 osGetPerfFrequancy(void); // boundary — os perf-counter tick rate

float apProfileTimer::GetResSec()
{
    double totalTicks = (double)this->totalTime;
    // Deviation: decompiler rendered osGetPerfFrequancy() as ">> 32". The disasm
    // (823CDEC4) passes the full 64-bit r3 result straight into __u64tod — no shift.
    double frequency = (double)osGetPerfFrequancy();
    this->totalTime = 0;
    return (float)(totalTicks / frequency);
}
