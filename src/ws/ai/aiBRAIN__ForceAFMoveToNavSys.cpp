// aiBRAIN::ForceAFMoveToNavSys @ 0x832B0498
// ?ForceAFMoveToNavSys@aiBRAIN@@MAA_NABV?$dsTSTRING@D@@0_N@Z  (protected virtual)
//
// "Force after-frame" scripted move onto nav system `nsName`: resolve the compound-path destination
// waypoint for "{nsName}", snap the body to that waypoint's origin, localize onto the returned nav
// system, and notify behaviour `bhvName` succeeded. Returns false when no destination waypoint could
// be resolved. `doPushBHV` is unused on this path (the move is applied immediately).
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/wpWP.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"       // m3dVZero

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

bool aiBRAIN::ForceAFMoveToNavSys(const dsTSTRING<char> &nsName, const dsTSTRING<char> &bhvName,
                                  bool /*doPushBHV*/)
{
    dsTSTRING<char> errHint; // GetCmpPathDestWP writes a reason here on failure

    dsTSTRING<char> dest;
    dsSPrintf(&dest, "{%s}", nsName.pBuffer->str);

    short   destWPID = -1;
    navSYS *destNS = nullptr;
    navWP  *wp = this->mind.goal.pointee->GetCmpPathDestWP(&dest, &destWPID, &destNS, 0, &errHint);
    if (!wp)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToNavSys(\"%s\", \"%s\"): %s",
               this->names.nameInst, nsName.pBuffer->str, bhvName.pBuffer->str, errHint.pBuffer->str);
        return false;
    }

    m3dV pos;
    wp->wpData->TransformPoint(&m3dVZero, &pos);
    this->Transport(&pos);
    if (destNS)
        this->mind.goal.pointee->SetNavSystem(destNS, true);

    _apLog("~AI~'%s' ForceAF call MoveToNavSys(\"%s\", \"%s\")",
           this->names.nameInst, nsName.pBuffer->str, bhvName.pBuffer->str);

    if (bhvName.pBuffer->strLen)
    {
        this->mind.task.pointee->NoticeBhvEndSuccess(0x1000000u, bhvName);
        this->OnBhvEnd(bhvName.pBuffer->str);
        this->OnBhvEndSuccess(bhvName.pBuffer->str);
    }
    return true;
}
