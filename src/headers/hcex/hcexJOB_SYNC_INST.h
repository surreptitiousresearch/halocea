#pragma once
#include "../ws/jbm/jbmJOB.h"
#include "../ws/os/osHANDLE_DUMMY.h"
/* hcexJOB_SYNC_INST -- one worker slot of the hcexJOB_SYNC_INST_MNG pool: a jbmJOB that, on each
 * job-manager dispatch, syncs a batch of animINST render matrices (via HCEX_OBJ) and then services
 * queued animINST cull/validate work batches until told to stop.
 * DB-verified layout (types_members hcexJOB_SYNC_INST): jbmJOB@0 (12B, base), ready@12
 * (osHANDLE_DUMMY*), start@16 (osHANDLE_DUMMY*), batches@20 (instRANGE[200], 1600B), batchIdx@1620
 * (int), batchNum@1624 (int) -- size 1628. */

struct hcexJOB_SYNC_INST_vtbl; // boundary -- same 5-slot shape as jbmJOB_vtbl (dtr/Execute/Pre/Post/OnAdd)

struct hcexJOB_SYNC_INST : jbmJOB
{
    // hcexJOB_SYNC_INST::instRANGE -- one [start,end) animINST index range queued for this worker.
    struct instRANGE
    {
        int start; // 0x00
        int end;   // 0x04
    };

    static hcexJOB_SYNC_INST_vtbl vftable; // `hcexJOB_SYNC_INST::`vftable'' -- boundary

    osHANDLE_DUMMY *ready;         // 0x0C -- signaled by Execute after each batch it drains
    osHANDLE_DUMMY *start;         // 0x10 -- signaled by AddInst/hcexJOB_SYNC_INST_MNG to wake Execute
    instRANGE       batches[200];  // 0x14 -- queued [instListStart,instListEnd) ranges (ring, see AddInstJob)
    int             batchIdx;      // 0x654 -- next unconsumed entry in `batches`
    int             batchNum;      // 0x658 -- one past the last queued entry in `batches`

    // 0x823D0ED0 -- default-construct: clear state, name "hcex_sync_obj", own vtable, fresh
    // batch cursors, and the two auto-reset signals (ready starts signaled, start starts clear).
    hcexJOB_SYNC_INST();

    // 0x823CE6A0 -- thunk: re-point the vtable back to the jbmJOB base before base teardown.
    ~hcexJOB_SYNC_INST();

    // 0x823D0F3C -- `vector deleting destructor': re-point the vtable to jbmJOB's, then
    // conditionally operator-delete `this` when bit 0 of deleteFlags is set.
    hcexJOB_SYNC_INST *VectorDeletingDtor(unsigned char deleteFlags);

    // 0x823CE708 -- queue an [instListStart,instListEnd) work range onto this worker's ring and
    // wake it via `start`. STRONG_ASSERT-crashes if `batches` is already full (batchNum >= 200).
    void AddInstJob(int instListStart, int instListEnd);

    // 0x823DD540 -- worker body: sync one HCEX_OBJ's render matrices per dispatch (round-robin via
    // the shared objIdxSyncMatr counter), then, while this is a render frame, repeatedly wait on
    // (stopJobs | start) and drain queued instance-range batches through
    // rendIsCullAndValidateInstNeeded/rendCullAndValidateInst until stopJobs wins the race.
    void Execute(int threadID);

    // 0x823CE6C8 -- true once `ready` is signaled (Execute has drained its current batch queue).
    bool JobDone();
};
