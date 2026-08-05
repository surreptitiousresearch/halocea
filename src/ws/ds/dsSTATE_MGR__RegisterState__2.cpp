#include "dsSTATE_MGR.h"
#include "dsSTRID.h"
#include "dsLF_SORTED_MAP.h"
#include "dsEVENT_MGR.h"
#include "../os/osLOCK.h"
#include "ds_assert_boundary.h"

extern "C" int sprintf_0(char *string, const char *format, ...); // boundary — CRT
extern int dsSTATE_MGR_GetThreadId();                                // boundary — reads the worker-thread slot from TLS (r13)
extern dsEVENT_MGR *gEventMgr;                             // boundary — process-wide event manager

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

typedef dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP> StrIntMap;

// dsSTATE_MGR::RegisterState(dsSTRID,bool) @ 0x8261AB28
// Register (or look up) a runtime state keyed by the interned id `id`. Fast path: return the
// existing index if the name is already in stateMap. Otherwise, under the lock (double-checked):
// allocate a fresh STATE_REC with no stored value (idxData = -1) and all per-thread value indices
// cleared to -1, optionally register an "On<name>Changed" change-notification event, publish
// {name -> index} into stateMap, and return the new state index.
int dsSTATE_MGR::RegisterState(dsSTRID id, bool isToggleEventOnChange)
{
    int threadId = dsSTATE_MGR_GetThreadId();
    if (!IGNORE_STRONG_ASSERT && (threadId < 0 || !this->threadInfo[threadId].isActive))
        STRONG_ASSERT_DUMMY().Crash(
            "GetThreadId() >= 0 && threadInfo[GetThreadId()].isActive",
            "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp",
            1106,
            empty_string);

    StrIntMap *stateMap = (StrIntMap *)&this->stateMap;
    osLOCK *lock = (osLOCK *)&this->lock;

    const char *name = id.CStr();
    const int *existing = stateMap->Find(&name);
    if (existing)
        return *existing;

    lock->Lock(nullptr, 0);

    name = id.CStr();
    existing = stateMap->Find(&name);
    if (existing) {
        int handle = *existing;
        lock->Unlock(nullptr, 0);
        return handle;
    }

    int stateIdx = this->stateIndex.Alloc();
    dsSTATE_MGR::STATE_REC &rec = this->stateIndex.Get(stateIdx);
    rec.idxData = -1;

    if (isToggleEventOnChange) {
        char eventName[320];
        sprintf_0(eventName, "On%sChanged", id.CStr());
        dsSTRID eventId(eventName, 0);
        rec.eventId = gEventMgr->RegisterEvent(eventId);
    } else {
        rec.eventId = -1;
    }

    for (int i = 0; i < 8; ++i)
        rec.idxThreadData[i] = -1;

    const char *key = id.CStr();
    stateMap->Insert(key, stateIdx);

    lock->Unlock(nullptr, 0);
    return stateIdx;
}
