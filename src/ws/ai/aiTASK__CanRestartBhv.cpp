// aiTASK::CanRestartBhv(unsigned int) @ 0x83262040
// ?CanRestartBhv@aiTASK@@UAA_NK@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// Virtual: true when some live task will restart behaviour `bhvSNum`. Suppressed while the brain is
// dead, the module is locked, or the brain status still has the AI_GEST "awaiting init" bit (0x20) set.
bool aiTASK::CanRestartBhv(unsigned int bhvSNum)
{
    if (pBrain->IsDead())
        return false;
    if (nLocks > 0)
        return false;
    if ((pBrain->st.pointee->gen.val >> 5) & 1)   // AI_GEST bit 5 — brain still awaiting init
        return false;

    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        if (!task->IsDone() && task->CanRestartBhv(bhvSNum))
            return true;
    }
    return false;
}
