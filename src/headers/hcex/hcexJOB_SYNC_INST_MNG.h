#pragma once
#include "hcexJOB_SYNC_INST.h"
#include "../ws/ds/dsCONST_ARRAY.h"
#include "../ws/anim/animINST.h"
/* hcexJOB_SYNC_INST_MNG -- owns the fixed pool of 4 hcexJOB_SYNC_INST workers and the shared
 * animINST worklist they drain. One global instance, `hcexJobSyncMng`, feeds work from
 * hcexJOB_SYNC_INST_MNG::AddInst (called once per eligible scene instance from StartSync, and
 * per-frame via the static PushInst thunk) and hands batches to workers in AddInst once enough
 * instances have queued up (instBatchSize) via round-robin worker selection.
 * DB-verified layout (types_members hcexJOB_SYNC_INST_MNG): stopJobs@0 (osHANDLE_DUMMY*),
 * objIdxSyncMatr@4 (int), instList@8 (animINST*[2500], 10000B), instListStart@10008 (int),
 * instListEnd@10012 (int), instBatchSize@10016 (int), lastUsedJob@10020 (int), jobs@10024
 * (dsCONST_ARRAY<hcexJOB_SYNC_INST,4>, 6516B) -- size 16540. Not itself polymorphic (no vtable
 * of its own -- its ctor/dtor/methods are all non-virtual per the mangled name qualifiers). */

typedef struct hcexJOB_SYNC_INST_MNG
{
    osHANDLE_DUMMY *stopJobs;         // 0x0000 -- manual-reset signal telling workers to stop draining
    int             objIdxSyncMatr;   // 0x0004 -- round-robin HCEX_OBJ index for render-matrix sync
    animINST       *instList[2500];   // 0x0008 -- pending animINST worklist (SCN_MAX_INST)
    int             instListStart;    // 0x2718 -- start of the not-yet-batched range in instList
    int             instListEnd;      // 0x271C -- end of the not-yet-batched range in instList
    int             instBatchSize;    // 0x2720 -- queue this many before handing a batch to a worker
    int             lastUsedJob;      // 0x2724 -- round-robin cursor into `jobs` (worker index 1..3)
    dsCONST_ARRAY<hcexJOB_SYNC_INST, 4> jobs; // 0x2728 -- jobs[0] runs inline in StopSync; 1..3 are workers

    // 0x823D2D1C -- default-construct: fresh cursors, instBatchSize 50, 4 worker slots, stopJobs
    // starts signaled (manual-reset).
    hcexJOB_SYNC_INST_MNG();

    // 0x823D0E8C -- destructor. DEVIATION: the decompiler mistyped the loop variable as
    // hcexJOB_SYNC_INST_MNG* walking backwards in 1628-byte (sizeof(hcexJOB_SYNC_INST)) strides;
    // it is really the compiler-generated per-element array destructor for `jobs.list[4]`,
    // resetting each worker's vtable back to jbmJOB's (the same reset hcexJOB_SYNC_INST::~
    // hcexJOB_SYNC_INST does for one element) before the array's storage is freed.
    ~hcexJOB_SYNC_INST_MNG();

    // 0x823D2D80 -- queue `pInst` onto the shared worklist (no-op if sync isn't in progress, or if
    // `jobs` has fewer than 2 entries). Once the not-yet-batched range reaches instBatchSize, pick
    // the next free/round-robin worker (skipping the inline worker 0) and hand it the whole range
    // via AddInstJob. A null `pInst` flushes the current range without queuing anything new
    // (used by StopSync to drain the tail).
    bool AddInst(animINST *pInst);

    // 0x823D4B38 -- static thunk: AddInst against the global `hcexJobSyncMng`.
    static bool PushInst(animINST *pInst);

    // 0x823D4B60 -- begin a sync pass: reset cursors, mark sync in progress, reset every worker's
    // batch cursors and `stopJobs`, hand workers 1/2/3 to the job manager (masks 4/0x10/0x20), then
    // walk the scene's live instance list and AddInst every eligible one (has a Blam-side HCEX_OBJ
    // sign, no owning entity, and the "needs sync" state2 bit 0x200000 set).
    void StartSync();

    // 0x823D4D64 -- end a sync pass: flush the tail range (AddInst(nullptr)), wait for render-frame
    // dispatch to finish if this is a render frame, signal `stopJobs`, run worker 0's Execute
    // inline (it never got a job-manager dispatch), wait for that to finish, clear the in-progress
    // flag, and drop a PIX marker with the count of instances left unqueued.
    void StopSync();
} hcexJOB_SYNC_INST_MNG;

extern hcexJOB_SYNC_INST_MNG hcexJobSyncMng;
