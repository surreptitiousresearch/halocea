// aiBRAIN::ForceAFMoveToPoint @ 0x832AFCD8
// ?ForceAFMoveToPoint@aiBRAIN@@MAA_NABV?$dsTSTRING@D@@0_N@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/wpWP.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

extern const m3dV m3dVZero;

// "Auto-finish" MoveToPoint: instead of walking a path, teleport straight to the compound-path
// destination waypoint (adopting its nav system when the destination lives in a different one) and
// fire the behaviour-end events. Returns false when no destination waypoint could be resolved.
bool aiBRAIN::ForceAFMoveToPoint(const dsTSTRING<char> &wpName, const dsTSTRING<char> &bhvName,
                                 bool doPushBHV)
{
    (void)doPushBHV; // consumed by the walking path only; the auto-finish path ignores it

    dsTSTRING<char> errHint;
    short   destWPID = -1;
    navSYS *destNS   = nullptr;
    navWP  *wp = mind.goal.pointee->GetCmpPathDestWP(&wpName, &destWPID, &destNS, false, &errHint);
    if (wp)
    {
        m3dV pos;
        wp->wpData->TransformPoint(&m3dVZero, &pos);
        Transport(&pos);
        if (destNS)
            mind.goal.pointee->SetNavSystem(destNS, true);
        _apLog("~AI~'%s' ForceAF call MoveToPoint(\"%s\", \"%s\")",
               names.nameInst, wpName.CStr(), bhvName.CStr());
        if (bhvName.pBuffer->strLen)
        {
            mind.task.pointee->NoticeBhvEndSuccess(0x1000000u, bhvName);
            OnBhvEnd(bhvName.CStr());
            OnBhvEndSuccess(bhvName.CStr());
        }
        return true;
    }

    _apLog("~AI,Ssl,Error~'%s' can't call MoveToPoint(\"%s\", \"%s\"): %s",
           names.nameInst, wpName.CStr(), bhvName.CStr(), errHint.CStr());
    mind.task.pointee->NoticeBhvEndFail(0x1000000u, bhvName);
    OnBhvEnd(bhvName.CStr());
    OnBhvEndFailed(bhvName.CStr());
    return false;
}
