#pragma once
#include "apCOUNTER.h"
// ws-engine ap: a call-counting performance counter — an apCOUNTER that also tracks a running
// total of calls made against it (used to instrument hot entry points like entENTITY::Register).
// DB-verified layout (types_members apCOUNTER_CALL): apCOUNTER@0 (32B, base), callsTotal@32
// (int) — size 36.

struct apCOUNTER_CALL_vtbl; // boundary — ap subsystem vtable

struct apCOUNTER_CALL : apCOUNTER {
    static apCOUNTER_CALL_vtbl vftable; // `apCOUNTER_CALL::`vftable'' -- boundary, not decompiled

    int callsTotal; // 0x20 running call count, bumped via osLockedIncrement

    // 0x8271AE30 — construct a named call-counter (units string "calls").
    apCOUNTER_CALL(const char *name);
};
