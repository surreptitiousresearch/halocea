// aiTASK::NoticeBhvEndSuccess(unsigned int, const dsTSTRING<char>&) @ 0x83261CE0
// ?NoticeBhvEndSuccess@aiTASK@@UAA_NKABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK.h"

// Virtual: a behaviour ended successfully — offer it to each live task until one consumes it.
// Returns whether the SSL "behaviour ended" event should still be raised (the consuming task's vote,
// or true when no task consumes the notification).
bool aiTASK::NoticeBhvEndSuccess(unsigned int bhvSNum, const dsTSTRING<char> &bhvName)
{
    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        if (!task->IsDone())
        {
            bool allowSSLEvent = true;
            if (task->NoticeBhvEndSuccess(pBrain, bhvSNum, &bhvName, &allowSSLEvent))
                return allowSSLEvent;
        }
    }
    return true;
}
