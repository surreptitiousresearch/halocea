#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/bhp/bhpPARAMS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <cstring>

// dsSPrintf @ 0x825210C0 — formatted build into a new dsTSTRING (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);
// _apLog @ engine logger. boundary.
extern void _apLog(const char *fmt, ...);

// aiBEST::PushBehaviorByParams @ 0x83298CC0
// ?PushBehaviorByParams@aiBEST@@UAA_NPAVbhpPARAMS@@HPBD@Z (virtual override)
//
// Build a behaviour of `pars->typeBehav`, tag it with the script id `SLID`, initialise it from
// `pars`, and push it. `isMajor` selects sub-behaviour flagging. Returns whether the behaviour is
// still live after the push (a behaviour that both FINISHED and FAILED reports failure).
bool aiBEST::PushBehaviorByParams(bhpPARAMS *pars, int isMajor, const char *SLID)
{
    aiBID typeBehav = pars->typeBehav;
    pars->isSubBehav = (isMajor == 0);

    bhvBEHAV *bhv = WrappedNewBehavior(typeBehav);
    if (!bhv)
    {
        _apLog("~AI,Error~%s: can't create behavior id %d",
               pBrain->names.nameInst, pars->typeBehav);
        return false;
    }

    strcpy(bhv->SLID, SLID ? SLID : "no script");
    bhv->Init(pars);

    dbgLine = dsSPrintf("SetByParams(%s) is in process.", bhv->SLID);

    dsTSTRING<char> bhvName = bhv->GetName();
    errorLine = dsSPrintf("~AI,Error~%s: rejected a push of behavior '%s'/%s",
                          pBrain->names.nameInst, bhv->SLID, bhvName.CStr());

    if (!PushBehavior(bhv))
    {
        _apLog(errorLine.CStr());
        return false;
    }

    unsigned int val = bhv->state.val;
    if ((val & BHV_ST_FINISHED) == 0)
        return true;
    if ((val & BHV_ST_FAILED) == 0)
        return true;
    return false;
}
