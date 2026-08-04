#pragma once
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "sslOBJ_REF.h"

struct sslSTACK; // ssl evaluation stack (full definition in sslSTACK.h)
// ssl subsystem: a queued (delayed) script function call. DB-verified layout
// (types_members sslDELAYED_FUNCTION): self@0, caller@4, funcIdx@8, params@12, timeLeft@32,
// group@36, pauseLocks@40, syncContext@44 — size 52.

typedef struct sslDELAYED_FUNCTION {
    sslOBJ_REF          self;        // 0x00 target object
    sslOBJ_REF          caller;      // 0x04 calling object
    int                 funcIdx;     // 0x08 function index (-1 when empty)
    dsVECTOR<dsDATA, 8> params;      // 0x0C call arguments
    float               timeLeft;    // 0x20 remaining delay
    dsSTRID             group;       // 0x24 grouping tag
    int                 pauseLocks;  // 0x28 pause reference count
    dsDATA              syncContext; // 0x2C sync context value

    // 0x82AF02E0 — reset to an empty/idle delayed call. QAA.
    void Clear();

    // 0x82AF0F90 — tick the pending call by `time` seconds: when the object is live, the record is
    // armed, unpaused and its timer has expired, fire the call (restoring the saved sync context
    // around it) and clear the record. Returns 1 (kept alive / not yet fired), 0 when it fired or is
    // suppressed this tick. QAA.
    int Update(float time);

    // 0x82AF4E18 — (re)initialise this record: bind self/caller/funcIdx/group/timer, snapshot the
    // SyncContext, and pop `argc` arguments off `stack` into `params`. Returns sslERROR by value (sret).
    sslERROR Setup(sslOBJ_REF self, int funcIdx, sslOBJ_REF caller, float timeOfs, int argc,
                   sslSTACK &stack, dsSTRID group);

    // 0x82AF0970 — construct an empty/idle record: default self/caller refs, empty params vector
    // (allocation-site cookie {"ssl_stack.cpp", 838}), null group id, null-type syncContext,
    // funcIdx -1 (empty), zero pauseLocks/timeLeft.
    sslDELAYED_FUNCTION();
} sslDELAYED_FUNCTION;
