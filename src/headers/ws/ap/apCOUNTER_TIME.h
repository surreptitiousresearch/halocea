#pragma once
#include "apCOUNTER.h"
// ap named cumulative call-time counters (profiling). apCOUNTER_TIME_RECUR (full definition:
// apCOUNTER_TIME_RECUR.h) is a static per-call-site recurring time counter; apCOUNTER_TIME_CALL_WRP
// is the RAII scope guard that accumulates elapsed time into it for the duration of one call
// (the WRP template itself is not decompiled here — only forward-uses COUNTER* by pointer).

struct apCOUNTER_TIME_RECUR; // full definition in apCOUNTER_TIME_RECUR.h
struct apCOUNTER_TIME_vtbl;  // boundary — ap subsystem vtable

// A single-shot (non-recursive) named time counter — an apCOUNTER (units "time") sampled once
// per frame via the Start/Stop idiom (see gsMsgCounterStart/Stop in hcexCallFrame.cpp).
// DB-verified layout (types_members apCOUNTER_TIME): apCOUNTER@0 (32B, base), customFreq@32
// (__int64), tmData@40 (tmDATA[8], 16B each), tmDataTotal@168 (tmDATA) — size 184.
struct apCOUNTER_TIME : apCOUNTER {
    static apCOUNTER_TIME_vtbl vftable; // `apCOUNTER_TIME::`vftable'' -- boundary, not decompiled

    // DB-verified (types_members apCOUNTER_TIME::tmDATA): start@0, sum@8 — size 16 (no `depth`,
    // unlike apCOUNTER_TIME_RECUR — this counter does not support re-entrant timing).
    struct tmDATA {
        __int64 start; // 0x00 sample start timestamp (osGetPerfCounter ticks)
        __int64 sum;   // 0x08 accumulated elapsed time
    };

    __int64 customFreq; // 0x20 custom timer frequency override (0 = use default)
    tmDATA  tmData[8];   // 0x28 per-processor-slot start/accumulate
    tmDATA  tmDataTotal; // 0xA8 all-time accumulation

    // Construct a named single-shot time counter (register it with the counter manager). The second
    // argument is the units-names string (nullptr → the default "time" units). boundary.
    apCOUNTER_TIME(const char *name, const char *unitsNames);
};

template<class COUNTER>
struct apCOUNTER_TIME_CALL_WRP {
    COUNTER *counter; // 0x00 the counter being accumulated into

    explicit apCOUNTER_TIME_CALL_WRP(COUNTER *c); // boundary — begin timing this scope
    ~apCOUNTER_TIME_CALL_WRP();                   // boundary — accumulate the elapsed time
};
