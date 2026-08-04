#pragma once
#include "strmSTREAMING_QUEUE.h"
#include "../ds/dsARRAY.h"
#include "../os/osLOCK.h"
// ws-engine strm (streaming): top-level streaming director — owns one strmSTREAMING_QUEUE per
// physical device (STRM_QUEUE_DVD, STRM_QUEUE_HDD) and routes each strmRESOURCE query to the
// queue matching its GetQueueId().
// DB-verified layout (types_members strmSTREAMING_MANAGER): queues@0
// (ds::ARRAY<strmSTREAMING_QUEUE,2>, 360B), frameCounter@360, lockAdd@364 (osLOCK) — size 416.

typedef struct strmSTREAMING_MANAGER {
    ds::ARRAY<strmSTREAMING_QUEUE, 2> queues; // 0x000
    int                                 frameCounter; // 0x168
    osLOCK                               lockAdd;       // 0x16C guards AddQuery's queue dispatch

    strmSTREAMING_MANAGER();  // 0x828F1310 — boundary, external to this batch
    ~strmSTREAMING_MANAGER(); // boundary

    void Clear(bool full);   // boundary — external to this batch
    void FlushQueries();      // boundary — external to this batch
    void DiskAccessLock();     // boundary — external to this batch

    // 0x828F3A60 — under `lockAdd`: dispatch to queues[resource->GetQueueId()].AddQuery.
    bool AddQuery(strmRESOURCE *resource);

    // 0x828F22F0 — dispatch to queues[resource->GetQueueId()].RemoveQuery (asserts the queue id
    // is in range first).
    bool RemoveQuery(strmRESOURCE *resource);
} strmSTREAMING_MANAGER;

extern strmSTREAMING_MANAGER strmMng; // ?strmMng@@3VstrmSTREAMING_MANAGER@@A — global instance
