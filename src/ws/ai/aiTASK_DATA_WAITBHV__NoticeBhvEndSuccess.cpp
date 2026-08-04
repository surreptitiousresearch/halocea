// aiTASK_DATA_WAITBHV::NoticeBhvEndSuccess @ 0x832603C8
// ?NoticeBhvEndSuccess@aiTASK_DATA_WAITBHV@@UAA_NPAVaiBRAIN@@KABV?$dsTSTRING@D@@AA_N@Z
#include "../../headers/ws/ai/aiTASK_DATA_WAITBHV.h"

// Virtual: a behaviour ended successfully. Always permit the SSL event; complete this wait when the
// ended behaviour is the one being awaited.
bool aiTASK_DATA_WAITBHV::NoticeBhvEndSuccess(aiBRAIN * /*pBrain*/, unsigned int /*bhvSNum*/,
                                              const dsTSTRING<char> &bhvName, bool &allowSSLEvent)
{
    allowSSLEvent = true;
    if (!(waitBhvName == bhvName))
        return false;
    isDone = true;
    return true;
}
