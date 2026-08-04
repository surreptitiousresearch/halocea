#pragma once
#include "dsSTRID.h"
#include "dsDATA.h"
#include "dsPOOL.h"
#include "../os/osLOCK.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTR_HASH.h"
struct dsSTR_CMP; // boundary — const char* ordering policy functor (opaque; template arg only)
// ws-engine ds: registry of named runtime states with per-thread bookkeeping.
// DB-verified layout (types_members dsSTATE_MGR) — size 2236. The remaining nested members
// (osLOCK / dsLF_SORTED_MAP / THREAD_INFO) are ds/os-subsystem containers (boundaries in this
// batch; they are kept as correctly-sized opaque storage so the class layout stays faithful.

typedef struct dsSTATE_MGR {
    // One registered-state record — DB-verified (types_members dsSTATE_MGR::STATE_REC), size 40.
    struct STATE_REC {
        int idxData;          // 0x00 index into the `states` pool (-1 = no value)
        int eventId;          // 0x04 change-notification event id
        int idxThreadData[8]; // 0x08 per-thread value indices
    };

    // Per-worker-thread bookkeeping — DB-verified (types_members dsSTATE_MGR::THREAD_INFO), size 8.
    struct THREAD_INFO {
        int                   isActive;     // 0x00 nonzero while the thread is registered
        volatile unsigned int safePassMask; // 0x04
    };

    osLOCK        lock;                   // 0x000 (52)
    dsLF_SORTED_MAP<char const *, int, dsSTR_HASH, dsSTR_CMP> stateMap; // 0x034 (36)
    dsPOOL<STATE_REC, 256, 256> stateIndex; // 0x058 per-state records
    dsPOOL<dsDATA, 256, 256>    states;     // 0x468 per-state stored values
    THREAD_INFO   threadInfo[8];          // 0x878 dsSTATE_MGR::THREAD_INFO[8]
    volatile unsigned int threadMask;     // 0x8B8

    // 0x824F7058 — register a state named by a C string (interns it, forwards below).
    int RegisterState(const char *id, bool isToggleEventOnChange);
    // 0x8261AB28 — register a state named by an interned id: look it up in stateMap (fast path,
    // then again under the lock); on a miss allocate a fresh STATE_REC (idxData=-1, all per-thread
    // indices=-1), optionally register an "On<name>Changed" event, publish {name -> index} into the
    // map, and return the new state index. Reversed in dsSTATE_MGR__RegisterState__2.cpp.
    int RegisterState(dsSTRID id, bool isToggleEventOnChange);

    // 0x82615D80 — fetch the stored value for state `id`; returns a shared empty dsDATA if unset.
    const dsDATA *GetState(int id);

    // 0x82615CD8 — clear the stored value for state `id` (marks its stateIndex record empty).
    void EraseState(int id);

    // 0x826193D0 — store `*data` as the calling thread's value for state `id`, publish it as the
    // state's current value (with a change event). Reversed in dsSTATE_MGR__SetState.cpp.
    void SetState(int id, const dsDATA *data);
} dsSTATE_MGR;

// The global runtime-state registry singleton. boundary — defined elsewhere.
extern dsSTATE_MGR *gStateMgr;
