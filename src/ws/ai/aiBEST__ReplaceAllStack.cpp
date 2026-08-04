#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/os/osLOCK.h"

extern osLOCK aiBESTLock;                 // ?aiBESTLock@@3VosLOCK@@A — global best-behaviour commit lock
extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBEST::ReplaceAllStack @ 0x83298678
// ?ReplaceAllStack@aiBEST@@AAAHPAVbhvBEHAV@@@Z
//
// Under the best-behaviour lock, erase the whole changeable stack (reason BHV_ST_MANUALLY_CANCELLED)
// and push `pBehav` as the sole behaviour. Returns 1 on success (trashing the popped behaviours
// immediately if the brain is backing off its process-frame), 0 if the push was rejected. A null
// `pBehav` is an internal error, logged and rejected.
int aiBEST::ReplaceAllStack(bhvBEHAV *pBehav)
{
    if (!pBehav) {
        _apLog("~AI,Error~%s: internal error - requested set of NULL.", pBrain->names.nameInst);
        return 0;
    }

    aiBESTLock.Lock(nullptr, 0);
    EraseAll(BHV_ST_MANUALLY_CANCELLED);
    if (AddToChangeable(pBehav)) {
        if (pBrain->dbg.IsBackOffProcessFrame())
            TrashPopped();
        aiBESTLock.Unlock(nullptr, 0);
        return 1;
    }
    aiBESTLock.Unlock(nullptr, 0);
    return 0;
}
