// aiTASK::NoticeBhvEndFail(unsigned int, const dsTSTRING<char>&) @ 0x83261E00
// ?NoticeBhvEndFail@aiTASK@@UAA_NKABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK.h"

// Virtual: a behaviour ended in failure — offer it to each live task until one consumes it.
bool aiTASK::NoticeBhvEndFail(unsigned int bhvSNum, const dsTSTRING<char> &bhvName)
{
    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        if (!task->IsDone())
        {
            bool allowSSLEvent = true;
            if (task->NoticeBhvEndFail(pBrain, bhvSNum, &bhvName, &allowSSLEvent))
                return allowSSLEvent;
        }
    }
    return true;
}
