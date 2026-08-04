#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "sslBREAKPOINT.h"
// ssl subsystem: a script source-text record. DB-verified layout (types_members sslSOURCE):
//   dsREF_COUNT@0, source@4, validBPLines@8, Breakpoints@28 — size 48. The dsVECTOR members
//   store T* so their element types stay incomplete here.

typedef struct sslSOURCE {
    dsREF_COUNT                 refCount;     // 0x00
    dsTSTRING<char>             source;       // 0x04 the source text
    dsVECTOR<int, 8>            validBPLines; // 0x08 lines that can hold breakpoints
    dsVECTOR<sslBREAKPOINT, 8>  Breakpoints;  // 0x1C active breakpoints

    // 0x825CAB58 — copy of the source text (returns a fresh dsTSTRING<char> by value). const (QBA).
    dsTSTRING<char> Get() const;

    // 0x82A374B8 — true if a breakpoint on `line` is active for `object_id` (the object-uid scope).
    // Reproduces the binary faithfully: the first breakpoint matching `line` wins and only its
    // object_uid is compared; the `class_id` argument is accepted but unused. QAA.
    bool IsBreakpoint(int line, int class_id, int object_id);

    // 0x82A37008 (compiled into the scalar-deleting-destructor entry point; no separate
    // ~sslSOURCE call site observed, so the compiler folded the real destructor body directly
    // into it) — free Breakpoints.pData and validBPLines.pData unconditionally, then drop
    // source's buffer ref (freeing it once the count hits 0).
    ~sslSOURCE();

    // 0x82A37008 — compiler-generated deleting-destructor entry point: runs ~sslSOURCE(), then
    // conditionally `operator delete(this)` when bit 0 of `deleteFlags` is set.
    sslSOURCE *scalarDeletingDtor(unsigned char deleteFlags);
} sslSOURCE;
