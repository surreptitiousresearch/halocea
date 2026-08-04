#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// dsSPrintf @ 0x825210C0 — formatted build into a new dsTSTRING (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);
// _apLog @ engine logger. boundary.
extern void _apLog(const char *fmt, ...);

// aiBEST::SetBehavior @ 0x83298BE0
// ?SetBehavior@aiBEST@@UAAHPAD@Z (virtual)
//
// Create the scripted behaviour named `namePS` and make it the whole stack. A behaviour that
// initialises to FINISHED is discarded (queued for deletion); if it also FAILED that is logged and
// reported as failure. A still-in-process behaviour replaces the entire stack.
int aiBEST::SetBehavior(char *namePS)
{
    bhvBEHAV *bhv = CreateScriptedBehavior(namePS);
    if (!bhv)
        return 0;

    unsigned int val = bhv->state.val;
    if (val & BHV_ST_FINISHED)
    {
        int failed = (val & BHV_ST_FAILED) != 0;
        AddToDel(&bhv);
        if (failed)
        {
            _apLog("~AI,Error~%s: SetBHV(%s) failed to init behavior.",
                   pBrain->names.nameInst, namePS);
            return 0;
        }
        return 1;
    }

    dbgLine = dsSPrintf("SetBHV(%s) is in process.", namePS);
    return ReplaceAllStack(bhv);
}
