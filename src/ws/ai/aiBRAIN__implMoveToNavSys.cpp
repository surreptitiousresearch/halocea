// aiBRAIN::implMoveToNavSys @ 0x832AFF20
// ?implMoveToNavSys@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@00_N@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner, dsSPrintf
#include "../../headers/ws/ai/aiBRAIN_DESC_abilities_boundary.h"
#include "../../headers/ws/bhp/bhpPATH_PAR.h"
#include "../../headers/ws/bhp/bhvPATH_boundary.h"
#include "../../headers/ws/nav/navPATH.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/ia/iaIACTOR.h" // spDesc
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog
#include <string.h>

// Move the brain onto the named nav system `nsName`, running behaviour `bhvName` (of movement
// `type`). No-op success when already in that nav system; back-off / missing-current-NS / unknown-NS
// / path-build failures return false. The AF status bit routes to a teleport (ForceAFMoveToNavSys).
bool aiBRAIN::implMoveToNavSys(const dsTSTRING<char> &nsName, const dsTSTRING<char> &bhvName,
                              const dsTSTRING<char> &type, bool doPushBHV)
{
    if (!aiBRAIN_DESC_AbilNS(spDesc.pointee))
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToNavSys(\"%s\", \"%s\"): NS feature is disabled",
               names.nameInst, nsName.CStr(), bhvName.CStr());
        return false;
    }

    dsTSTRING<char> callName = dsSPrintf("MoveToNavSys(\"%s\", \"%s\")", nsName.CStr(), bhvName.CStr());
    dsFLAGS<SSL_COMMON_BACKOFF, int> backoffChecks;
    backoffChecks.val = 3;
    if (IsSSLCommonBackOff(callName, backoffChecks))
        return false;

    // AF (auto-finish) status bit (0x02000000) → teleport onto the nav system instead.
    if ((st.pointee->mind.val >> 25) & 1)
        return ForceAFMoveToNavSys(nsName, bhvName, doPushBHV);

    navSYS *curNS = mind.goal.pointee->GetNavSystem();
    if (!curNS)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToNavSys(\"%s\", \"%s\"): no current nav sys!",
               names.nameInst, nsName.CStr(), bhvName.CStr());
        return false;
    }

    // Already in the requested nav system — nothing to do.
    if (strcmp(nsName.CStr(), curNS->nameSys.CStr()) == 0)
    {
        if (bhvName.pBuffer->strLen)
        {
            OnBhvEnd(bhvName.CStr());
            OnBhvEndSuccess(bhvName.CStr());
        }
        _apLog("~AI~'%s' call MoveToNavSys(\"%s\", \"%s\"), already in NS",
               names.nameInst, nsName.CStr(), bhvName.CStr());
        return true;
    }

    navSYS *targetNS = aiPlanner->GetNavManager()->FindNavSys(nsName.CStr());
    if (!targetNS)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToNavSys(\"%s\", \"%s\"): nav sys not found!",
               names.nameInst, nsName.CStr(), bhvName.CStr());
        return false;
    }

    bhpPATH_PAR par;
    dsTSTRING<char> errHint;
    if (!mind.goal.pointee->BuildPathToNS(-1, nullptr, nullptr, targetNS,
                                          reinterpret_cast<navPATH *>(par.pathData), &errHint))
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToNavSys(\"%s\", \"%s\"): %s",
               names.nameInst, nsName.CStr(), bhvName.CStr(), errHint.CStr());
        return false;
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
            _apLog("~AI,Ssl,Error~'%s' MoveToNavSys(\"%s\", \"%s\"): can't push PATH bhv",
                   names.nameInst, nsName.CStr(), bhvName.CStr());
            return false;
        }
    }
    else if (!mind.best.pointee->SetBehaviorByParams(&par, name))
    {
        _apLog("~AI,Ssl,Error~'%s' MoveToNavSys(\"%s\", \"%s\"): can't set PATH bhv",
               names.nameInst, nsName.CStr(), bhvName.CStr());
        return false;
    }

    _apLog("~AI~'%s' call MoveToNavSys(\"%s\", \"%s\")",
           names.nameInst, nsName.CStr(), bhvName.CStr());
    return true;
}
