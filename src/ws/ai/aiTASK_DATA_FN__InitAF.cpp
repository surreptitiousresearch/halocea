#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include <string.h> // memcpy

// aiTASK_DATA_FN::InitAF @ 0x8325F810
// ?InitAF@aiTASK_DATA_FN@@UAAX_N0P8aiBRAIN@@BA_NABV?$dsVECTOR@VdsDATA@@$07@@@Z@Z (virtual)
//
// Configure the task's after-fire result checking: record the fn-result and bhv-result check flags,
// and when neither is requested disable the inherited force-AF flag; then store the force-AF checker
// delegate into the 16-byte forceAFfnChecker slot.
void aiTASK_DATA_FN::InitAF(bool checkFnResult_, bool checkBhvResults_, aiForceAFfnChecker forceAFfnChecker_)
{
    this->checkFnResult   = checkFnResult_;
    this->checkBhvResults = checkBhvResults_;
    if (!checkFnResult_ && !checkBhvResults_)
        this->afForceEnabled = false; // aiTASK_DATA::afForceEnabled @ 0x0C

    // 16-byte PPC pointer-to-member-function stored verbatim into the __int128 slot @0x58.
    memcpy(this->forceAFfnChecker, &forceAFfnChecker_, sizeof(this->forceAFfnChecker));
}
