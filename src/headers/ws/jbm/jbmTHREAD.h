#pragma once
#include "../os/osLOCK.h"
#include "../ds/dsDEQUE.h"
#include "jbmJOB.h"
// ws-engine jbm: one worker thread owned by jbmMANAGER. Runs jobs pushed to its
// per-thread deque, or executes a job inline when the caller is already running on
// the target processor.
// DB-verified layout (types_members jbmTHREAD): procID@0 (int), threadId@4
// (osHANDLE_DUMMY*), startThread@8 (osHANDLE_DUMMY*), threadDone@12 (osHANDLE_DUMMY*),
// lock@16 (osLOCK, 52), jobDeque@68 (dsDEQUE<jbmJOB*>, 32) — size 100.

struct osHANDLE_DUMMY; // boundary — opaque OS wait/thread handle

typedef struct jbmTHREAD {
    int                  procID;      // 0x00 processor index this thread is pinned to
    osHANDLE_DUMMY      *threadId;    // 0x04 OS thread handle
    osHANDLE_DUMMY      *startThread; // 0x08 signal: wake worker to drain jobDeque
    osHANDLE_DUMMY      *threadDone;  // 0x0C signal: worker's deque has drained
    osLOCK               lock;        // 0x10 guards jobDeque
    dsDEQUE<jbmJOB *>     jobDeque;    // 0x44 pending jobs for this thread

    // 0x826EBA90 — run pJob to completion on this thread: PreExecute, Execute, PostExecute,
    // in that vtable order (verified against disasm: vtbl+8, vtbl+4, vtbl+0xC).
    void ExecuteJob(jbmJOB *pJob);

    // 0x826EC800 — hand pJob to this thread. If the caller is already running on this
    // thread's processor, runs it inline via ExecuteJob(); otherwise locks jobDeque, pushes
    // the job, and signals startThread so the worker wakes and drains it.
    void AddJob(jbmJOB *pJob);
} jbmTHREAD;
