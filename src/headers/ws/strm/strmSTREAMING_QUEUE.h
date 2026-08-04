#pragma once
#include "strmRESOURCE.h"
#include "../ds/dsVECTOR.h"
#include "../os/osLOCK.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/NAMED_PAIR.h"
// ws-engine strm (streaming): a priority worklist of pending strmRESOURCE loads for one physical
// device queue (DVD or HDD — see STRM_QUEUE_ID), serviced by a dedicated worker thread.
// DB-verified layout (types_members strmSTREAMING_QUEUE): queries@0 (dsVECTOR<strmRESOURCE*,8>),
// lock@20 (osLOCK), diskAccessLock@72 (osLOCK), threadExist@124, hThread@128, threadId@132,
// hEventStrmThreadOn@136, hEventStrmThreadOff@140, id@144, enabled@148, queueUpdateCallbacks@152
// (dsCONST_ARRAY<..., 3>, 28B) — size 180.

struct osHANDLE_DUMMY; // ../fio/osASYNC_INFO.h — boundary

typedef struct strmSTREAMING_QUEUE {
    dsVECTOR<strmRESOURCE *, 8> queries; // 0x00 pending loads for this device queue
    osLOCK                       lock;             // 0x14 guards `queries`
    osLOCK                       diskAccessLock;    // 0x48
    bool                          threadExist;        // 0x7C
    unsigned char                 _pad7D[3];          // 0x7D db-verified padding
    osHANDLE_DUMMY               *hThread;             // 0x80
    int                            threadId;             // 0x84
    osHANDLE_DUMMY                *hEventStrmThreadOn;   // 0x88 signaled while the worker should run
    osHANDLE_DUMMY                *hEventStrmThreadOff;  // 0x8C signaled while the worker should idle
    int                             id;                    // 0x90 this queue's STRM_QUEUE_ID
    bool                             enabled;                // 0x94
    unsigned char                    _pad95[3];              // 0x95 db-verified padding
    // 0x98 (28B) — per-queue update-notification callback slots (DB member type spelled verbatim).
    dsCONST_ARRAY<ds::NAMED_PAIR<ds::NAMED_PAIR_callback<void(*)(dsVECTOR<strmRESOURCE *,8> const &,void *)>,ds::NAMED_PAIR_data<void *> >,3> queueUpdateCallbacks;

    strmSTREAMING_QUEUE();  // 0x828F0F70 — boundary (outside this re-source)
    ~strmSTREAMING_QUEUE(); // boundary

    void Clear();       // boundary (outside this re-source)
    void ClearQueue();  // boundary (outside this re-source)
    void DiskAccessLock();   // boundary (outside this re-source)
    void DiskAccessUnlock(); // boundary (outside this re-source)

    // 0x828F3860 — lazily spawn (once) the "streaming" worker thread running ThreadProc(this) at
    // priority 1.
    void CreateThread();

    // 0x828EFBF0 — toggle the worker thread's run/idle signal pair
    // (hEventStrmThreadOn/hEventStrmThreadOff) and record the new `enabled` state.
    void EnableStrmThread(bool enable);

    // 0x828F38C8 — under `lock`: bail out if the resource is already in RES_STATE_LOADING; drop
    // any existing queue entry for it, then (if OnAddQuery accepts) push it, set
    // RES_STATE_IN_QUERY, and (re)start the worker thread. Returns whether the resource had NOT
    // already been queued (mirrors the binary's `v19 >> 31` on the pre-erase Find result).
    bool AddQuery(strmRESOURCE *resource);

    // 0x828F2048 — under `lock`: drop the resource's queue entry if present and set
    // RES_STATE_CANCEL, then (outside the lock) spin on osSleep(1) while the resource is still
    // RES_STATE_LOADING. Always returns true.
    bool RemoveQuery(strmRESOURCE *resource);

private:
    // 0x828EFxxx — the worker thread entry point spawned by CreateThread. boundary (outside this re-source)
    // 
    static void ThreadProc(strmSTREAMING_QUEUE *self);
} strmSTREAMING_QUEUE;
