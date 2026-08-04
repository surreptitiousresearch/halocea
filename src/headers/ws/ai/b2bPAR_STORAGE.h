#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine b2b: a behaviour's "parcel" storage — the shared per-behaviour data blackboard.
// DB-verified layout (types_members b2bPAR_STORAGE) — size 24: __vftable@0, storage@4
// (dsVECTOR<b2bPARCEL*,8>). Methods reached by the aiBEST batch are declared as boundaries.

struct b2bPARCEL;             // b2b — a single parcel (fwd)
struct b2bPAR_STORAGE_vtbl;   // vtable (fwd)

struct b2bPAR_STORAGE {
    b2bPAR_STORAGE_vtbl      *__vftable; // 0x00
    dsVECTOR<b2bPARCEL *,8>   storage;   // 0x04

    // Age all stored parcels' timers by `dt`. boundary — body external to this batch.
    void UpdateTime(float dt);
    // Propagate a commit-changes pass into the stored parcels (`phase` selects sub-step).
    // boundary — body external to this batch.
    void UpdateCommChange(int phase);
    // Advance the storage one frame (`phase` selects sub-step). boundary — body external.
    void UpdateFrame(int phase);
    // Drop parcels whose lifetime has elapsed. boundary — body external.
    void RemoveExpired();
};
