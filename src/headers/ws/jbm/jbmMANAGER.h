#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsCONST_ARRAY.h"
#include "jbmTHREAD.h"
// ws-engine jbm: the job manager -- owns a fixed pool of up to 16 worker threads and
// routes jobs to them by processor-affinity mask.
// DB-verified layout (types_members jbmMANAGER): threads@0
// (dsCONST_ARRAY<jbmTHREAD,16>, 1604) — size 1604.

struct jbmJOB; // jbmJOB.h — job base (pointer only)

typedef struct jbmMANAGER {
    dsCONST_ARRAY<jbmTHREAD, 16> threads; // 0x000 worker pool, indexed by processor

    // 0x826ECAE0 — route pJob to every live thread whose bit is set in threadMask.state.
    // Bit 28 is a "current processor" wildcard: when set, it is replaced by the bit for
    // the calling thread's own processor (osGetCurThreadProcessor()) before the scan.
    // Iterates thread indices from (threads.nEntry-1) down to 0, calling
    // jbmTHREAD::AddJob() on each live (startThread != nullptr) thread whose index bit is
    // in the mask.
    // 0x826ECBE8 — grow the worker pool by nAdditionalThreads (?AddThreads@jbmMANAGER@@QAAXH@Z).
    void AddThreads(int nAdditionalThreads);

    void AddJob(apSTATE_T<unsigned long> threadMask, jbmJOB *pJob);

    // 0x826EBE08 — block until every live thread whose bit is set in threadMask.state has
    // drained its jobDeque (osWaitForSignal on threadDone, infinite timeout). Iterates
    // thread indices from (threads.nEntry-1) down to 0; each in-mask index is bounds
    // checked (STRONG_ASSERT-style IsValidIdx) before the dsCONST_ARRAY access.
    void WaitThreadsDone(apSTATE_T<unsigned long> threadMask);
} jbmMANAGER;
