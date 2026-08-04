// aiBRAIN::implMoveToPoint @ 0x832AF850
// ?implMoveToPoint@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@00_N@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/bhp/bhpPATH_PAR.h"
#include "../../headers/ws/bhp/bhvPATH_boundary.h"
#include "../../headers/ws/nav/navPATH.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // dsSPrintf
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// Move the brain along a compound nav path to waypoint `wpName`, running behaviour `bhvName` (of the
// movement `type`). When the AF ("auto-finish") status bit is set the move teleports instead
// (ForceAFMoveToPoint). Returns true when the move was started (or was a no-op because the body is
// already there); false on back-off or path-build failure.
bool aiBRAIN::implMoveToPoint(const dsTSTRING<char> &wpName, const dsTSTRING<char> &bhvName,
                              const dsTSTRING<char> &type, bool doPushBHV)
{
    // Reject the call when the brain is in a common SSL back-off state.
    dsTSTRING<char> callName = dsSPrintf("MoveToPoint(\"%s\", \"%s\")", wpName.CStr(), bhvName.CStr());
    dsFLAGS<SSL_COMMON_BACKOFF, int> backoffChecks;
    backoffChecks.val = 3;
    if (IsSSLCommonBackOff(callName, backoffChecks))
        return false;

    // AF (auto-finish) status bit (0x02000000) → teleport straight to the destination instead.
    if ((st.pointee->mind.val >> 25) & 1)
        return ForceAFMoveToPoint(wpName, bhvName, doPushBHV);

    bhpPATH_PAR par;
    dsTSTRING<char> errHint;
    if (!mind.goal.pointee->BuildCompoundPath(wpName, reinterpret_cast<navPATH *>(par.pathData),
                                              false, errHint))
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToPoint(\"%s\", \"%s\"): %s",
               names.nameInst, wpName.CStr(), bhvName.CStr(), errHint.CStr());
        return false;
    }

    navPATH *pathData = reinterpret_cast<navPATH *>(par.pathData);
    if (pathData->pathWP.nElem == 0)
    {
        // Empty path — the body is already at the destination: report success immediately.
        mind.task.pointee->NoticeBhvEndSuccess(0x1000000u, bhvName);
        OnBhvEnd(bhvName.CStr());
        OnBhvEndSuccess(bhvName.CStr());
        mind.task.pointee->bhvNotNeed = true;
        return true;
    }

    par.isLoop = 0;
    par.isShoot = 1;
    par.ParseSpeed(this, const_cast<dsTSTRING<char> *>(&type));
    par.useHOPath = &bhvPATH::UseHO_CombatStyle;
    if (st.pointee->mind.val & 0x1000000)
        par.notifyTaskSys = true;

    const char *name = bhvName.pBuffer->strLen ? bhvName.CStr() : nullptr;
    if (doPushBHV)
    {
        if (!mind.best.pointee->PushBehaviorByParams(&par, 1, name))
        {
            _apLog("~AI,Ssl,Error~'%s' MoveToPoint(\"%s\", \"%s\"): can't push PATH bhv",
                   names.nameInst, wpName.CStr(), bhvName.CStr());
            return false;
        }
    }
    else if (!mind.best.pointee->SetBehaviorByParams(&par, name))
    {
        _apLog("~AI,Ssl,Error~'%s' MoveToPoint(\"%s\", \"%s\"): can't set PATH bhv",
               names.nameInst, wpName.CStr(), bhvName.CStr());
        return false;
    }

    _apLog("~AI~'%s' call MoveToPoint(\"%s\", \"%s\")",
           names.nameInst, wpName.CStr(), bhvName.CStr());
    return true;
}
