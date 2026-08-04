#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h"

// aiPLANNER::DbgReasonWaitSetupInitial(dsTSTRING<char>&) — 0x83166000
// Debug helper: report (into `reason`) why initial AI setup is still being deferred, returning
// true when a blocking condition applies. Order matches the disassembly: game still loading
// (gsAppState bit1 clear) -> no player -> unfarming-after-vis-change -> else ask the nav manager.
bool aiPLANNER::DbgReasonWaitSetupInitial(dsTSTRING<char> &reason)
{
    if (((~gsAppState >> 1) & 1) != 0)
    {
        dsTSTRING<char> s;
        s.pBuffer = nullptr;
        s.UnsafeInit("aiPLANNER::Game loading", -1, 0);
        reason = s;
        if (--s.pBuffer->refCount == 0)
            dlFree(s.pBuffer);
        return true;
    }

    if (!gsGetPlayer(0))
    {
        dsTSTRING<char> s;
        s.pBuffer = nullptr;
        s.UnsafeInit("aiPLANNER::No player", -1, 0);
        reason = s;
        if (--s.pBuffer->refCount == 0)
            dlFree(s.pBuffer);
        return true;
    }

    if (farmSys->isUnfarmingAfterVisChange)
    {
        dsTSTRING<char> s;
        s.pBuffer = nullptr;
        s.UnsafeInit("aiPLANNER::Unfarming After Vis Change", -1, 0);
        reason = s;
        if (--s.pBuffer->refCount == 0)
            dlFree(s.pBuffer);
        return true;
    }

    return navManager->DbgReasonWaitSetupInitial(&reason);
}
