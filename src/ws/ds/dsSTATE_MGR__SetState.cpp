// dsSTATE_MGR::SetState @0x826193D0
#include "dsSTATE_MGR.h"
#include "dsDATA.h"
#include "dsPARAM.h"
#include "dsPARAM_LIST.h"
#include "dsEVENT_MGR.h"
#include "../os/osLOCK.h"
#include "ds_assert_boundary.h"

// boundary — current lock-free-container thread ordinal [0,32).
int dsSTATE_MGR_GetThreadId();   /* ?GetThreadId@dsSTATE_MGR@@ @0x82612D78; disambiguated from the dsERASE_MANAGER twins */
// boundary — full memory fence.
extern "C" void osMemoryBarrier();
// boundary — the global event manager singleton.
extern dsEVENT_MGR *gEventMgr;
// boundary — the interned "value" parameter id passed with the change event.
extern "C" dsSTRID valId;

// dsSTATE_MGR::SetState @ 0x826193D0
// Store `*data` as the calling thread's value for state `id`: lazily allocate the per-thread value
// slot (under the lock), write it, publish it as the state's current value with a memory barrier,
// and — if the state has a change event — signal it carrying the new value as the "value" parameter.
void dsSTATE_MGR::SetState(int id, const dsDATA *data)
{
    if (!IGNORE_STRONG_ASSERT) {
        int threadId = dsSTATE_MGR_GetThreadId();
        if (threadId < 0 || !this->threadInfo[threadId].isActive)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "GetThreadId() >= 0 && threadInfo[GetThreadId()].isActive",
                "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp",
                1147,
                empty_string);
    }

    int threadId = dsSTATE_MGR_GetThreadId();
    dsSTATE_MGR::STATE_REC *rec = &this->stateIndex.Get(id);
    if (rec->idxThreadData[threadId] < 0) {
        ((osLOCK *)&this->lock)->Lock(nullptr, 0);
        rec->idxThreadData[threadId] = this->states.Alloc();
        ((osLOCK *)&this->lock)->Unlock(nullptr, 0);
    }

    dsDATA *slot = &this->states.Get(rec->idxThreadData[threadId]);
    slot->StoreValue(*data);
    osMemoryBarrier();

    int eventId = rec->eventId;
    rec->idxData = rec->idxThreadData[threadId]; // publish the per-thread value as the current value
    if (eventId >= 0) {
        dsSTRID paramId;               // stack dsSTRID (default-constructed)
        dsDATA  paramValue;            // stack dsDATA — the "value" parameter
        dsPARAM_LIST params;
        paramValue.type = nullptr;
        params.list = (dsPARAM *)&paramId; // paramId/paramValue are contiguous: a dsPARAM {id,value}
        paramId.id = valId.id;
        paramValue.SetValue(*data);
        params.length = 1;
        gEventMgr->SignalEvent(rec->eventId, &params);
        if (paramValue.type)
            paramValue.type->Destroy(&paramValue.storage);
    }
}
