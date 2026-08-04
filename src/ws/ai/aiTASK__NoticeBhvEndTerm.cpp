// aiTASK::NoticeBhvEndTerm(unsigned int, const dsTSTRING<char>&) @ 0x83261F20
// ?NoticeBhvEndTerm@aiTASK@@UAA_NKABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK.h"

// Virtual: a behaviour was terminated — offer it to each live task until one consumes it.
bool aiTASK::NoticeBhvEndTerm(unsigned int bhvSNum, const dsTSTRING<char> &bhvName)
{
    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        if (!task->IsDone())
        {
            bool allowSSLEvent = true;
            if (task->NoticeBhvEndTerm(pBrain, bhvSNum, &bhvName, &allowSSLEvent))
                return allowSSLEvent;
        }
    }
    return true;
}
