#pragma once
#include <stdint.h>
// ws-engine ap: lightweight wall-clock profiler. Accumulates elapsed hardware-timer
// ticks into totalTime; GetResSec converts to seconds and resets the accumulator.
// DB-verified layout (types_members apProfileTimer): startTime@0, totalTime@8 — size 16.

// High-resolution timer frequency (ticks/sec) — Saber os primitive, boundary.
extern "C" uint64_t osGetPerfFrequancy(void);

typedef struct apProfileTimer {
    uint64_t startTime; // 0x00 tick stamp of the last Start()
    uint64_t totalTime; // 0x08 accumulated ticks since last read

    // 0x823CDEA0 (src/ws/ap/apProfileTimer__GetResSec.cpp) — convert the accumulated ticks to
    // seconds (osGetPerfFrequancy units) and clear the accumulator.
    float GetResSec();
} apProfileTimer;
