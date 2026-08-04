#include "../../headers/ws/ai/aiHO_SYS_BASE.h"

// aiHO_SYS_BASE::FindHOClosestTo @ 0x8317A690
// ?FindHOClosestTo@aiHO_SYS_BASE@@UAA_NFFFAAV?$dsVECTOR@F$07@@@Z
//
// Base default stub: reset `closestArr` and return `wpidFrom` as the sole result. The disassembly's
// leading loop over the incoming array is a no-op; wpidParent/wpidTo are ignored. Returns true.
// (The decompiler's a6..a15 parameters are PPC register noise — the ABI (mangle) takes only the
// four arguments below.)
bool aiHO_SYS_BASE::FindHOClosestTo(short wpidParent, short wpidFrom, short wpidTo,
                                   dsVECTOR<short, 8> &closestArr)
{
    (void)wpidParent; (void)wpidTo;
    short result = wpidFrom;
    closestArr.nElem = 0;
    closestArr.PushBack(result);
    return true;
}
