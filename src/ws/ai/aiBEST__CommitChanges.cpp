#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/os/osLOCK.h"

extern osLOCK aiBESTLock; // ?aiBESTLock@@3VosLOCK@@A — global best-behaviour commit lock

// aiBEST::CommitChanges @ 0x832975D0
// ?CommitChanges@aiBEST@@AAAXXZ
//
// Apply all pending stack edits in one guarded pass (re-entrancy is suppressed by the
// COMMCHG_IN_PROCESS flag): under the global best lock, reset sparsings if anything is queued for
// deletion or awaiting activation, trash popped behaviours, activate new ones, and commit the parcel
// storage change pass.
void aiBEST::CommitChanges()
{
    unsigned int val = state.val;
    if (val & BEST_ST_COMMCHG_IN_PROCESS)
        return;

    state.val = val | BEST_ST_COMMCHG_IN_PROCESS;
    aiBESTLock.Lock(nullptr, 0);

    if (arrToDel.nEntry > 0 || FindNonActivated())
        pBrain->ResetAllSparsings();

    TrashPopped();
    ActivateNewBehaviors();
    parcelStorage.UpdateCommChange(1);

    state.val &= ~BEST_ST_COMMCHG_IN_PROCESS;
    aiBESTLock.Unlock(nullptr, 0);
}
