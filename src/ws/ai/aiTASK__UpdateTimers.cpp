// aiTASK::UpdateTimers @ 0x832619F0
// ?UpdateTimers@aiTASK@@UAAXM@Z
#include "../../headers/ws/ai/aiTASK.h"

// Virtual: advance the timers of every live task in order. Stops at the first still-active task that
// is a blocker (later tasks in the queue must wait for it). Done tasks are skipped.
void aiTASK::UpdateTimers(float time)
{
    int count = tasks.nElem;
    for (int i = 0; i < count; ++i) {
        aiTASK_DATA *task = tasks[i];
        if (task->IsDone())
            continue;
        task->UpdateTimers(time);
        if (task->IsBlocker())
            break;
    }
}
