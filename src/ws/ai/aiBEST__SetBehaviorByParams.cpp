#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/bhp/bhpPARAMS.h"
#include <cstring>

extern void _apLog(const char *fmt, ...);              // ?_apLog@@YAXPBDZZ — engine logger  boundary
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // 0x825210C0 — format into a fresh dsTSTRING (sret)

// aiBEST::SetBehaviorByParams @ 0x83298930
// ?SetBehaviorByParams@aiBEST@@UAA_NPAVbhpPARAMS@@PBD@Z  (virtual override)
//
// Build the behaviour described by `pars` (a non-sub behaviour), tag it with descriptor `SLID`
// (or "no script"), initialise it, and make it the sole behaviour on the stack (ReplaceAllStack).
// Returns true on success. On failure to create the behaviour object, or when ReplaceAllStack
// rejects it, an error is logged and false is returned.
bool aiBEST::SetBehaviorByParams(bhpPARAMS *pars, const char *SLID)
{
    pars->isSubBehav = 0;
    bhvBEHAV *behav = WrappedNewBehavior(pars->typeBehav);
    if (!behav) {
        _apLog("~AI,Error~%s: can't create behavior id %d", pBrain->names.nameInst, pars->typeBehav);
        return false;
    }

    strcpy(behav->SLID, SLID ? SLID : "no script");
    behav->Init(pars);

    dbgLine   = dsSPrintf("SetByParams(%s) is in process.", behav->SLID);
    dsTSTRING<char> name = behav->GetName();
    errorLine = dsSPrintf("~AI,Error~%s: rejected a set of behavior '%s'/%s",
                          pBrain->names.nameInst, behav->SLID, name.CStr());

    if (ReplaceAllStack(behav))
        return true;

    _apLog(errorLine.CStr());
    return false;
}
