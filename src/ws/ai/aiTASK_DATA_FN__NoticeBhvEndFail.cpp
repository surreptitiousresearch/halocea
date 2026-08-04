// aiTASK_DATA_FN::NoticeBhvEndFail(aiBRAIN*, unsigned int, const dsTSTRING<char>&, bool&) @ 0x83262AD0
// ?NoticeBhvEndFail@aiTASK_DATA_FN@@UAA_NPAVaiBRAIN@@KABV?$dsTSTRING@D@@AA_N@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"

// Virtual: a fired behaviour failed. When awaiting the reply or in the after-fire window and this is
// one of our fired behaviours, restart the after-fire pass and consume the SSL event (clear
// allowSSLEvent, return true). Otherwise leave the event to propagate.
bool aiTASK_DATA_FN::NoticeBhvEndFail(aiBRAIN *pBrain, unsigned int bhvSNum,
                                     const dsTSTRING<char> & /*bhvName*/, bool &allowSSLEvent)
{
    allowSSLEvent = true;
    bool inFireableState = (state == AI_TASK_DATA_FN_WAIT_REPLY || state == AI_TASK_DATA_FN_WAIT_AF);
    if (!inFireableState || !HandleBhvSNum(bhvSNum))
        return false;

    allowSSLEvent = false;
    AFRestart(pBrain);
    return true;
}
