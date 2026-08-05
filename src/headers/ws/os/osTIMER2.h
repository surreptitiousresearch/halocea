#pragma once
#include <stdint.h>
// ws-engine os: high-resolution elapsed-time tracker, DB-verified layout (types_members osTIMER2).
// Reset() snapshots the current performance counter; Snapshot() converts (perfcounter - start) into
// MILLISECONDS via the platform perf-frequency (Xenon time-base), latches it into `time`, and returns
// it. Used by sslCLASS::CallFunc for call profiling. Reset() body not decompiled here (boundary).

typedef struct osTIMER2 {
    uint64_t start; // 0x00 performance-counter value captured by Reset()
    float             time;  // 0x08 last computed elapsed time, in milliseconds

    void  Reset();     // boundary — capture the current performance counter into `start`
    float Snapshot();  // non-const per mangle (QAAMXZ) — elapsed ms since the last Reset()
} osTIMER2;
