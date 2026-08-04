// aiTASK::IsNeedFillEmptyBhvStack() const @ 0x83261B00
// ?IsNeedFillEmptyBhvStack@aiTASK@@UBA_NXZ
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// Virtual const: the brain's empty behaviour stack should be refilled unless a live (non-done) task
// is ready to create a behaviour. A live blocking task also forces a refill.
bool aiTASK::IsNeedFillEmptyBhvStack() const
{
    if (!tasks.nElem)
        return true;
    if (pBrain->IsDead())
        return true;
    if (nLocks > 0)
        return true;

    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        if (task->IsDone())
            continue;                 // done tasks don't participate
        if (task->CanCreateBhv())
            return false;             // a live task will supply a behaviour — no refill needed
        if (task->IsBlocker())
            return true;              // a live blocker forces a refill
        // live, can't create, not a blocker: keep scanning
    }
    return true;
}
