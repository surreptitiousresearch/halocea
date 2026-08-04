// aiTASK::Clear() @ 0x83263BC8
// ?Clear@aiTASK@@UAAXXZ
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// Virtual: queue every live task for deferred destruction, then empty the active task list.
void aiTASK::Clear()
{
    int nElem = this->tasks.nElem;
    for (int i = 0; i < nElem; ++i)
        this->tasksDelQueue.PushBack(this->tasks[i]);

    // The active list is dropped without per-element teardown here — the delete queue owns the tasks now.
    this->tasks.nElem = 0;
}
