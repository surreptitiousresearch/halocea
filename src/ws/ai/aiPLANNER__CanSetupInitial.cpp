#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h"

// aiPLANNER::CanSetupInitial(aiBRAIN*, dsTSTRING<char>*) — 0x83165F48
// While the world is unfarming after a visibility change, initial setup is disallowed (with an
// optional reject reason). Otherwise defer to the nav manager.
bool aiPLANNER::CanSetupInitial(aiBRAIN *pBrain, dsTSTRING<char> *rejectReason)
{
    if (!farmSys->isUnfarmingAfterVisChange)
        return navManager->CanSetupInitial(pBrain, rejectReason);

    if (rejectReason)
    {
        dsTSTRING<char> reason;
        reason.pBuffer = nullptr;
        reason.UnsafeInit("Unfarming After Vis Change", -1, 0);
        *rejectReason = reason;
        if (--reason.pBuffer->refCount == 0)
            dlFree(reason.pBuffer);
    }
    return false;
}
