#pragma once
#include "dsDEQUE.h"
#include "../os/osLOCK.h"

// ws-engine ds: deferred-erase manager for lock-free containers. Queues elements to be
// destroyed once no thread holds a reference, guarded by an access lock.
// DB-verified layout (types_members
//   dsERASE_MANAGER<dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>):
//   threadAllocMask@0, eraseQueue@4 (dsDEQUE, 32 bytes), accessLock@36 (osLOCK, 52 bytes).
// Template param ERASER = the container-specific eraser policy (e.g. LINE_ERASER).

template<class ERASER>
struct dsERASE_MANAGER {
    // The queued erase record (dsERASE_MANAGER<...>::ERASED in the DB) — DB-verified size 12:
    // threadMask@0, eraser@4 (the container-specific ERASER cursor).
    struct ERASED {
        unsigned int threadMask; // 0x00 dedup/ownership key (holds the erased LINE pointer)
        ERASER       eraser;     // 0x04 the erase cursor itself
    };

    unsigned int      threadAllocMask; // 0x00 per-thread allocation ownership bits
    dsDEQUE<ERASED>   eraseQueue;      // 0x04 pending erases (32 bytes)
    osLOCK            accessLock;      // 0x24 serialises queue access (52 bytes)

    // Register the calling thread with the manager before a lock-free traversal; body not
    // reversed here.
    void CheckThread(); // boundary

    // 0x82529478 — lazily assign the calling thread a per-manager allocation slot the first time
    // it participates: if the thread has no slot yet, take the access lock, find the lowest free
    // bit in threadAllocMask, claim it, and record the slot index in the thread's TLS record.
    void RegisterThread();

    // 0x8252B368 / 0x8261A2E8 — enqueue `eraser` for deferred destruction under the access lock.
    void SafelyEraseObject(const ERASER &eraser);

    // 0x8252B1A8 / 0x8261A128 — drain/reclaim queued erases for the calling thread: under the
    // access lock, free every front entry that all OTHER registered threads have already
    // acknowledged (popping it from the ring), then mark the remaining entries as seen by this
    // thread. No-op when the calling thread has no allocation slot. Reversed per-instantiation.
    void UpdateThread();

    // 0x82529EB0 / 0x82615FF0 — construct an empty manager (zeroed queue + fresh lock).
    dsERASE_MANAGER();
};
