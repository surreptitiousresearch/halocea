#pragma once
#include "apCOUNTER_TIME_CALL.h"
#include "apCOUNTER_TIME_RECUR.h"
#include "apCOUNTER_UNIT.h"
#include "../ds/dsVECTOR.h"
#include "apPROFILER.h"
// ws-engine ap: a scope-guard wrapper that starts a counter's timing on construction and stops
// it on destruction (RAII timing block), parameterised by the counter kind being wrapped.
// DB-verified layout (types_members apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL>): counter@0
// (T*) — size 4.

template<class T>
struct apCOUNTER_TIME_CALL_WRP {
    T *counter; // 0x00 wrapped counter

    // Reversed per-instantiation (apCOUNTER_TIME_CALL, apCOUNTER_TIME_RECUR): starts unit-0/
    // unit-1 timing for `cnt` when their state's 0x40 "late-added" bit is set (promoted to
    // "started" bit 0x02 for the remainder of this scope), and pokes any bound apPROFILER's
    // Start(). apCOUNTER_TIME_CALL additionally bumps callsTotal; apCOUNTER_TIME_RECUR
    // additionally tracks a per-frame-slot recursion depth (only records the start timestamp on
    // the outermost (depth<=0) entry).
    apCOUNTER_TIME_CALL_WRP(T *cnt);

    // Reversed per-instantiation: mirror-image of the ctor — stops unit-0/unit-1 timing
    // (accumulating elapsed time into tmData[curProcessor].sum), pokes any bound apPROFILER's
    // Stop(). apCOUNTER_TIME_RECUR only actually stops the timer once its recursion depth
    // unwinds back to 0.
    ~apCOUNTER_TIME_CALL_WRP();
};
