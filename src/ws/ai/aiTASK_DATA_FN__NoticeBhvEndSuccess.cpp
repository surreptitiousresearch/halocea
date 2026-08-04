// aiTASK_DATA_FN::NoticeBhvEndSuccess(aiBRAIN*, unsigned int, const dsTSTRING<char>&, bool&) @ 0x83262A68
// ?NoticeBhvEndSuccess@aiTASK_DATA_FN@@UAA_NPAVaiBRAIN@@KABV?$dsTSTRING@D@@AA_N@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"

// Virtual: a fired behaviour finished successfully. While awaiting the SSL reply, account `bhvSNum`
// against the after-fire behaviour list; once that list drains the task is complete. Never consumes
// the SSL "behaviour ended" event (returns false, keeps allowSSLEvent set).
bool aiTASK_DATA_FN::NoticeBhvEndSuccess(aiBRAIN * /*pBrain*/, unsigned int bhvSNum,
                                        const dsTSTRING<char> & /*bhvName*/, bool &allowSSLEvent)
{
    allowSSLEvent = true;
    if (state == AI_TASK_DATA_FN_WAIT_REPLY && HandleBhvSNum(bhvSNum) && !afBhvList.nElem)
        state = AI_TASK_DATA_FN_DONE;
    return false;
}
