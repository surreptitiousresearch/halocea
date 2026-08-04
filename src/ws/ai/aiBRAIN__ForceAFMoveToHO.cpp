// aiBRAIN::ForceAFMoveToHO @ 0x832B1510
// ?ForceAFMoveToHO@aiBRAIN@@MAA_NABV?$dsTSTRING@D@@0_N@Z  (protected virtual)
//
// "Force after-frame" scripted move onto hideout `hoName`: resolve the compound-path destination HO
// waypoint, snap the body there, localize onto the returned nav system, acquire the HO edge (when it
// has no children yet) and set-or-push a MAJOR_HO behaviour tagged `bhvName`+"_HO". Returns false and
// notifies behaviour failure when the destination cannot be resolved or the behaviour cannot be
// set/pushed; true and notifies success otherwise.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // AI_BID_MAJOR_HO
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/wpWP.h"
#include "../../headers/ws/bhp/bhpHO_PAR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"       // m3dVZero

bool aiBRAIN::ForceAFMoveToHO(const dsTSTRING<char> &hoName, const dsTSTRING<char> &bhvName,
                              bool doPushBHV)
{
    dsTSTRING<char> errHint;

    short   destWPID = -1;
    navSYS *destNS = nullptr;
    navWP  *wp = this->mind.goal.pointee->GetCmpPathDestWP(&hoName, &destWPID, &destNS, 1, &errHint);
    if (!wp)
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToHO(\"%s\", \"%s\"): %s",
               this->names.nameInst, hoName.pBuffer->str, bhvName.pBuffer->str, errHint.pBuffer->str);
        this->mind.task.pointee->NoticeBhvEndFail(0x1000000u, bhvName);
        this->OnBhvEnd(bhvName.pBuffer->str);
        this->OnBhvEndFailed(bhvName.pBuffer->str);
        return false;
    }

    m3dV pos;
    wp->wpData->TransformPoint(&m3dVZero, &pos);
    this->Transport(&pos);
    if (destNS)
        this->mind.goal.pointee->SetNavSystem(destNS, true);

    dsTSTRING<char> bhvHO = bhvName + "_HO";

    bool acquired = false;
    if (!wp->wpData->HaveChilds())
    {
        wp->AcquireHO(this);
        acquired = true;
    }

    bhpHO_PAR par;
    par.typeBehav = AI_BID_MAJOR_HO;
    par.timeToExecute = 0.0f;
    par.wpid = destWPID;

    aiBEST_BASE *best = this->mind.best.pointee;
    // DEVIATION: the set/push argument tuple is not recoverable from the decompiler (call shown with
    // no operands); reconstructed from the sibling pattern — the built "<bhv>_HO" tag names the
    // behaviour, push takes the sub-behaviour flag 1.
    bool ok = doPushBHV ? best->PushBehaviorByParams(&par, 1, bhvHO.pBuffer->str)
                        : best->SetBehaviorByParams(&par, bhvHO.pBuffer->str);
    if (!ok)
    {
        if (doPushBHV)
            _apLog("~AI,Ssl,Error~'%s' MoveToHO(\"%s\", \"%s\"): can't push HO bhv",
                   this->names.nameInst, hoName.pBuffer->str, bhvName.pBuffer->str);
        else
            _apLog("~AI,Ssl,Error~'%s' MoveToHO(\"%s\", \"%s\"): can't set HO bhv",
                   this->names.nameInst, hoName.pBuffer->str, bhvName.pBuffer->str);
        if (acquired)
            wp->ReleaseHO(this);
        this->mind.task.pointee->NoticeBhvEndFail(0x1000000u, bhvName);
        this->OnBhvEnd(bhvName.pBuffer->str);
        this->OnBhvEndFailed(bhvName.pBuffer->str);
        return false;
    }

    _apLog("~AI~'%s' ForceAF call MoveToHO(\"%s\", \"%s\")",
           this->names.nameInst, hoName.pBuffer->str, bhvName.pBuffer->str);
    if (bhvName.pBuffer->strLen)
    {
        this->mind.task.pointee->NoticeBhvEndSuccess(0x1000000u, bhvName);
        this->OnBhvEnd(bhvName.pBuffer->str);
        this->OnBhvEndSuccess(bhvName.pBuffer->str);
    }
    return true;
}
