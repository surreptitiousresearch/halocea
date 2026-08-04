#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine ai08 — a queued "strike performance load" sample tracked by aiLOD_SYS. Each entry adds
// `load` to the system's cached load sum for `timeLife` seconds; `timerToDie` counts down from
// `timeLife` and the sample is dropped when it expires.
// DB-verified layout (types_members aiS_PERF_LOAD): load@0, timerToDie@4, timeLife@8,
// nameBrain@0xC, nameStrike@0x10 — size 20.

struct aiS_PERF_LOAD {
    const float     load;       // 0x00 load contribution
    float           timerToDie; // 0x04 remaining lifetime (s)
    const float     timeLife;   // 0x08 total lifetime (s)
    dsTSTRING<char> nameBrain;  // 0x0C originating brain name
    dsTSTRING<char> nameStrike; // 0x10 originating strike name

    // ?0aiS_PERF_LOAD@@QAA@MMPBD0@Z — build a sample from (load, timeLife, brain name, strike name).
    // boundary — body external to this batch.
    aiS_PERF_LOAD(float load, float timeLife, const char *nameBrain, const char *nameStrike);
    ~aiS_PERF_LOAD();
};
