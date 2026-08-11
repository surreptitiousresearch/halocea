// dsSTATE_MGR::EraseState @0x82615CD8
#include "dsSTATE_MGR.h"
#include "ds_assert_boundary.h"

// Current ws worker-thread id (read from the thread-local control block); boundary — not reversed.
extern int dsSTATE_MGR_GetThreadId();   /* ?GetThreadId@dsSTATE_MGR@@ @0x82612D78; disambiguated from the dsERASE_MANAGER twins */

// dsSTATE_MGR::EraseState @ 0x82615CD8
// Clear the stored value for state `id` by marking its stateIndex record empty (idxData = -1).
// The debug precondition asserts the caller runs on a registered, active worker thread.
void dsSTATE_MGR::EraseState(int id)
{
    if (!IGNORE_STRONG_ASSERT) {
        int threadId = dsSTATE_MGR_GetThreadId();
        if (threadId < 0 || !this->threadInfo[threadId].isActive)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "GetThreadId() >= 0 && threadInfo[GetThreadId()].isActive",
                "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp",
                1180,
                empty_string);
    }
    this->stateIndex.Get(id).idxData = -1;
}
