#pragma once
#include "apCOUNTER.h"
// ws-engine ap: a named recurring time counter — an apCOUNTER (units "time") that supports
// re-entrant/nested timing (each tmDATA slot tracks a recursion `depth`), optionally bound to
// an apPROFILER for live-graph display.
// DB-verified layout (types_members apCOUNTER_TIME_RECUR): apCOUNTER@0 (32B, base),
// customFreq@32 (__int64), tmData@40 (tmDATA[8]), tmDataTotal@200 (tmDATA) — size 220.

struct apCOUNTER_TIME_RECUR_vtbl; // boundary — ap subsystem vtable
struct apPROFILER;                // boundary — ap subsystem, only used by pointer here

struct apCOUNTER_TIME_RECUR : apCOUNTER {
    static apCOUNTER_TIME_RECUR_vtbl vftable; // `apCOUNTER_TIME_RECUR::`vftable'' -- boundary, not decompiled

    // DB-verified (types_members apCOUNTER_TIME_RECUR::tmDATA): start@0, sum@8, depth@16 — size 20.
    struct tmDATA {
        __int64 start; // 0x00 sample start timestamp
        __int64 sum;   // 0x08 accumulated elapsed time
        int     depth; // 0x10 recursion depth (re-entrant timing)
    };

    __int64 customFreq;  // 0x20 custom timer frequency override (0 = use default)
    tmDATA  tmData[8];   // 0x28 per frame-history-slot accumulation + recursion depth
    tmDATA  tmDataTotal;  // 0xC8 all-time accumulation

    // 0x8271AA98 — construct: apCOUNTER base (units "time"), zero customFreq/tmData/tmDataTotal;
    // if `prof` is non-null, bind counterUnits[0] to it (myProfiler, prof->currentUnit) and set
    // the unit's 0x10 "has profiler" state bit.
    apCOUNTER_TIME_RECUR(const char *name, apPROFILER *prof);
};
