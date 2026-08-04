#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_boundaries.h"

// aiWATCHER_BRAIN::NoticeBeingOnPath @ 0x83170220
// ?NoticeBeingOnPath@aiWATCHER_BRAIN@@UAAXABUm3dV@@@Z
// Virtual override. Forward the on-path notice to the tracked brain's "best" mind module, unless
// the brain's aiSTATUS suppression bit (bit 2 counting from the MSB of the status flag word) is
// set. The forward is a virtual call on aiBEST_BASE (vtable slot 0x98) through a computed pointer
// (aiBEST_BASE's layout is the next frontier).
void aiWATCHER_BRAIN::NoticeBeingOnPath(const m3dV &dirOnPath)
{
    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;

    const char *st = reinterpret_cast<const char *>(pBrain->st.pointee);
    unsigned int flags = *reinterpret_cast<const unsigned int *>(st + aiSTATUS_off_flags);
    if ((flags >> 29) & 1) // bit 2 from MSB — on-path suppression
        return;

    void *best = *reinterpret_cast<void *const *>(
        reinterpret_cast<const char *>(pBrain) + aiBRAIN_off_mindBestPointee);
    void **vtbl = *reinterpret_cast<void ***>(best);
    typedef void (*FnNoticeBeingOnPath)(void *self, const m3dV *dir);
    FnNoticeBeingOnPath notify = reinterpret_cast<FnNoticeBeingOnPath>(
        vtbl[aiBEST_BASE_vslot_NoticeBeingOnPath / sizeof(void *)]);
    notify(best, &dirOnPath);
}
