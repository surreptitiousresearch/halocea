#pragma once
#include "propENT.h"
#include "../ds/dsTYPE_ID.h"
// ws-engine prop: bridges a legacy Blam/HCEX halo object into the ws-engine property system.
// DB-verified layout (types_members propHALO_OBJ): <propENT base>@0 (40), timeTerm@40,
// funcOut@44 (float[4]), funcIn@60 (float[4]) — size 76.

// This class's own vtable slot. Opaque — not expanded; only its address is taken. // boundary
struct propHALO_OBJ_vtbl;

// Descriptor for propHALO_OBJ — full layout lives in propHALO_OBJ_DESC.h. Include the canonical
// header rather than redefining a minimal shim here (avoids C2011 when both are co-included).
#include "propHALO_OBJ_DESC.h"

struct propHALO_OBJ : propENT {
    float timeTerm;    // 0x28 absolute gsLvlTimer time this property was terminated (OwnerTerm)
    float funcOut[4];  // 0x2C output function-curve sample (4 channels)
    float funcIn[4];   // 0x3C input function-curve sample (4 channels)

    static propHALO_OBJ_vtbl vftable; // `propHALO_OBJ::`vftable'' — boundary, not decompiled

    // 0x823EC2D8 — base-construct, install this class's vtable, zero timeTerm, and default every
    // funcOut/funcIn channel to 1.0 (verified against the disassembly's stfs/stfsu store pair,
    // which the decompiler rendered as a confusing HIDWORD-style pointer walk).
    propHALO_OBJ();

    // 0x823EC340 (virtual, thunk) — straight through to propBASE::~propBASE (no vtable reset or
    // extra teardown of its own: propHALO_OBJ adds only POD float fields).
    ~propHALO_OBJ();

    // 0x823EC348 (virtual) — compiler-generated deleting-destructor entry point (vector-deleting
    // variant; DB shows only the (this, flags) shape, no array-count parameter).
    propHALO_OBJ *vectorDeletingDtor(unsigned char deleteFlags);

    // 0x823EB418 — mark termination time and, if the descriptor's configured timeTerm is ~0
    // (immediate-terminate), post a terminate message (2) to the owning actor right away.
    void OwnerTerm();

    // 0x823EB460 (virtual) — once timeTerm has been set (OwnerTerm) and the descriptor's
    // configured grace period has elapsed, post a terminate message (2) to the owning actor.
    void ProcessFRAME();
};
