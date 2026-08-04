// aiTASK_DATA_WAITBHV::NoticeBhvEndTerm @ 0x83260488
// ?NoticeBhvEndTerm@aiTASK_DATA_WAITBHV@@UAA_NPAVaiBRAIN@@KABV?$dsTSTRING@D@@AA_N@Z
#include "../../headers/ws/ai/aiTASK_DATA_WAITBHV.h"

// Virtual: a behaviour was terminated. Always permit the SSL event; complete this wait when the
// ended behaviour is the one being awaited.
bool aiTASK_DATA_WAITBHV::NoticeBhvEndTerm(aiBRAIN * /*pBrain*/, unsigned int /*bhvSNum*/,
                                           const dsTSTRING<char> &bhvName, bool &allowSSLEvent)
{
    allowSSLEvent = true;
    if (!(waitBhvName == bhvName))
        return false;
    isDone = true;
    return true;
}
