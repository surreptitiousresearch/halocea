#pragma once
#include <stdint.h>
#include "apCOUNTER.h"
// ws-engine ap: a named cumulative call-time counter — an apCOUNTER (units "time/calls") that
// accumulates elapsed-time samples per frame-history slot plus a running call count.
// DB-verified layout (types_members apCOUNTER_TIME_CALL): apCOUNTER@0 (32B, base),
// customFreq@32 (__int64), tmData@40 (tmDATA[8]), tmDataTotal@168 (tmDATA), callsTotal@184
// (int) — size 188.

struct apCOUNTER_TIME_CALL_vtbl; // boundary — ap subsystem vtable

struct apCOUNTER_TIME_CALL : apCOUNTER {
    static apCOUNTER_TIME_CALL_vtbl vftable; // `apCOUNTER_TIME_CALL::`vftable'' -- boundary, not decompiled

    // DB-verified (types_members apCOUNTER_TIME_CALL::tmDATA): start@0, sum@8 — size 16.
    struct tmDATA {
        int64_t start; // 0x00 sample start timestamp
        int64_t sum;   // 0x08 accumulated elapsed time
    };

    int64_t customFreq;      // 0x20 custom timer frequency override (0 = use default)
    tmDATA  tmData[8];       // 0x28 per frame-history-slot accumulation
    tmDATA  tmDataTotal;     // 0xA8 all-time accumulation
    int     callsTotal;      // 0xB8 running call count

    // 0x8271A930 — construct: apCOUNTER base (units "time/calls"), zero customFreq/tmData/
    // tmDataTotal/callsTotal.
    apCOUNTER_TIME_CALL(const char *name);
};
