#pragma once
// ws-engine ap: lightweight wall-clock profiler. Accumulates elapsed hardware-timer
// ticks into totalTime; GetResSec converts to seconds and resets the accumulator.
// DB-verified layout (types_members apProfileTimer): startTime@0, totalTime@8 — size 16.

// High-resolution timer frequency (ticks/sec) — Saber os primitive, boundary.
extern "C" unsigned __int64 osGetPerfFrequancy(void);

typedef struct apProfileTimer {
    unsigned __int64 startTime; // 0x00 tick stamp of the last Start()
    unsigned __int64 totalTime; // 0x08 accumulated ticks since last read

    // Return accumulated time in seconds and clear the accumulator.
    float GetResSec();
} apProfileTimer;
