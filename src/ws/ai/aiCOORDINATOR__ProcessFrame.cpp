#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiDEBUG.h"   // aiDEBUG::IsOffAI

// aiCOORDINATOR::ProcessFrame @ 0x83237E70
// ?ProcessFrame@aiCOORDINATOR@@UAAXM@Z
//
// Per-frame coordinator update (skipped while AI is disabled): tick every group, garbage-collect
// dead groups, and refresh the debug-render snapshot when the console's watched brain changed.
void aiCOORDINATOR::ProcessFrame(float dt)
{
    if (aiDEBUG::IsOffAI())
        return;

    int nElem = groups.nElem;
    for (int i = 0; i < nElem; ++i)
        groups[i]->ProcessFrame(dt);

    RemoveDeletedGroups();

    if (currentWatchee != _pBrainDebugWatchee)
    {
        currentWatchee = _pBrainDebugWatchee;
        UpdateDebugRenderInfo();
    }
}
