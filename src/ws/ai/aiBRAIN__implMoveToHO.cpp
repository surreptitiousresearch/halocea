// aiBRAIN::implMoveToHO @ 0x832B0E90
// ?implMoveToHO@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@00_N@Z  (virtual)
//
// SSL "MoveToHO(ho, arrive, style, run)" body: build a compound path to hideout `ho`, resolve and
// acquire the destination HO waypoint, then set/push a MAJOR_HO behaviour plus a PATH behaviour that
// walks there. In scripted-teleport mode (mind bit 25) forwards to ForceAFMoveToHO. Returns false on
// any path-build / HO-resolution / behaviour set/push failure.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // AI_BID_MAJOR_HO
#include "../../headers/ws/nav/navPATH.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/wpWP.h"
#include "../../headers/ws/bhp/bhpPATH_PAR.h"
#include "../../headers/ws/bhp/bhpHO_PAR.h"
#include "../../headers/ws/bhp/bhvPATH_boundary.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

bool aiBRAIN::implMoveToHO(const dsTSTRING<char> &ho, const dsTSTRING<char> &arrive,
                           const dsTSTRING<char> &style, bool run)
{
    dsTSTRING<char> callName;
    if (this->IsSSLCommonBackOff(
            *dsSPrintf(&callName, "MoveToHO(\"%s\", \"%s\")", ho.pBuffer->str, arrive.pBuffer->str),
            dsFLAGS<SSL_COMMON_BACKOFF, int>{7}))
        return false;

    if (((unsigned int)this->st.pointee->mind.val >> 25) & 1)
        return this->ForceAFMoveToHO(ho, arrive, run);

    bhpPATH_PAR pathPar;
    navPATH *path = reinterpret_cast<navPATH *>(pathPar.pathData);

    dsTSTRING<char> errHint;
    if (!this->mind.goal.pointee->BuildCompoundPath(ho, path, true, errHint))
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToHO(\"%s\", \"%s\"): %s",
               this->names.nameInst, ho.pBuffer->str, arrive.pBuffer->str, errHint.pBuffer->str);
        return false;
    }

    int lastIdx = path->pathWP.nElem - 1;
    short wpID = path->GetWP_ID(lastIdx);
    unsigned short edgeFlags[8];
    unsigned int blockedFlags;
    path->GetEdgeFlags(lastIdx, edgeFlags, &blockedFlags);

    navWP *ho_wp = aiGOAL_BASE::GetHO(wpID);
    if (!ho_wp || !ho_wp->CanAcquireHO(this, blockedFlags))
    {
        _apLog("~AI,Ssl,Error~'%s' can't call MoveToHO(\"%s\", \"%s\"): can't find target HO WP or HO is busy/disabled",
               this->names.nameInst, ho.pBuffer->str, arrive.pBuffer->str);
        return false;
    }

    dsTSTRING<char> bhvHO = arrive + "_HO";
    const char *pathName = arrive.pBuffer->strLen ? arrive.pBuffer->str : nullptr;

    pathPar.isLoop = 0;
    pathPar.isShoot = 1;
    pathPar.ParseSpeed(this, const_cast<dsTSTRING<char> *>(&style));
    pathPar.useHOPath = &bhvPATH::UseHO_CombatStyle;
    pathPar.PreparePathToHO();

    bool acquired = false;
    if (!ho_wp->wpData->HaveChilds())
    {
        ho_wp->AcquireHO(this);
        acquired = true;
    }

    bhpHO_PAR hoPar;
    hoPar.wpid = wpID;
    hoPar.typeBehav = AI_BID_MAJOR_HO;
    hoPar.timeToExecute = 0.0f;
    hoPar.flagsBlockedWP = blockedFlags;
    if ((this->st.pointee->mind.val & 0x1000000) != 0)
        pathPar.notifyTaskSys = true;

    aiBEST_BASE *best = this->mind.best.pointee;
    // DEVIATION: the HO-param set/push argument tuple is not recoverable from the decompiler (call
    // shown with no operands); reconstructed from the sibling pattern (the built "<bhv>_HO" tag).
    bool hoOK = run ? best->PushBehaviorByParams(&hoPar, 1, bhvHO.pBuffer->str)
                    : best->SetBehaviorByParams(&hoPar, bhvHO.pBuffer->str);
    if (!hoOK)
    {
        if (run)
            _apLog("~AI,Ssl,Error~'%s' MoveToHO(\"%s\", \"%s\"): can't push HO bhv",
                   this->names.nameInst, ho.pBuffer->str, arrive.pBuffer->str);
        else
            _apLog("~AI,Ssl,Error~'%s' MoveToHO(\"%s\", \"%s\"): can't set HO bhv",
                   this->names.nameInst, ho.pBuffer->str, arrive.pBuffer->str);
        if (acquired)
            ho_wp->ReleaseHO(this);
        return false;
    }

    if (!best->PushBehaviorByParams(&pathPar, 1, pathName))
    {
        _apLog("~AI,Ssl,Error~'%s' MoveToHO(\"%s\", \"%s\"): can't push PATH bhv",
               this->names.nameInst, ho.pBuffer->str, arrive.pBuffer->str);
        return false;
    }

    _apLog("~AI~'%s' call MoveToHO(\"%s\", \"%s\")",
           this->names.nameInst, arrive.pBuffer->str, ho.pBuffer->str);
    return true;
}
