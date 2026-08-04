// aiTASK::ProcessFrame() @ 0x83262B60
// ?ProcessFrame@aiTASK@@UAAXXZ
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// Virtual: advance the task queue one frame. Reap done tasks (deleting-dtor + erase), execute live
// tasks in order until one blocks/fails/locks the module or queues a deletion, then flush the
// deferred-destruction queue. No-op while the brain is dead, locked, or awaiting init.
void aiTASK::ProcessFrame()
{
    if (pBrain->IsDead() || nLocks > 0 || ((pBrain->st.pointee->gen.val >> 5) & 1) != 0)
        return;

    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/aft-task", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> timerScope(&localTimeCounter);

    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; )
    {
        aiTASK_DATA *task = tasks[i];
        if (task->IsDone())
        {
            task->Destroy();          // vtbl slot 0 deleting destructor (delete-flag = 1)
            tasks.Erase(i, 1);
            --nElem;
        }
        else
        {
            if (!task->Execute(pBrain, this) || task->IsBlocker() || nLocks > 0 ||
                tasksDelQueue.nElem)
            {
                break;
            }
            ++i;
        }
    }

    // Flush the deferred-destruction queue (tasks removed while iterating above).
    int nDel = tasksDelQueue.nElem;
    for (int i = 0; i < nDel; ++i)
    {
        aiTASK_DATA *deferred = tasksDelQueue[i];
        if (deferred)
            deferred->Destroy();      // deleting destructor
    }
    tasksDelQueue.nElem = 0;
}
