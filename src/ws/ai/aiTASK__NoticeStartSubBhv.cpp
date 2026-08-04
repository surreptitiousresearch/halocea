// aiTASK::NoticeStartSubBhv(unsigned int, unsigned int) @ 0x83261BF0
// ?NoticeStartSubBhv@aiTASK@@UAAXKK@Z
#include "../../headers/ws/ai/aiTASK.h"

// Virtual: forward a sub-behaviour start (parent -> child SNum) to every live task.
void aiTASK::NoticeStartSubBhv(unsigned int parentBhvSNum, unsigned int bhvSNum)
{
    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        if (!task->IsDone())
            task->NoticeStartSubBhv(parentBhvSNum, bhvSNum);
    }
}
