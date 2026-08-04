// aiTASK_DATA_WAITBHV::CheckDone @ 0x8325FF50
// ?CheckDone@aiTASK_DATA_WAITBHV@@IAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_WAITBHV.h"

// Mark done and return true when `bhvName` is the awaited behaviour.
bool aiTASK_DATA_WAITBHV::CheckDone(const dsTSTRING<char> &bhvName)
{
    if (!(waitBhvName == bhvName))
        return false;
    isDone = true;
    return true;
}
