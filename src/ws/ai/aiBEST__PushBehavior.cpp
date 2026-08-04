#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// _apLog @ engine logger. boundary.
extern void _apLog(const char *fmt, ...);

// aiBEST::PushBehavior @ 0x83298AF0
// ?PushBehavior@aiBEST@@AAAHPAVbhvBEHAV@@@Z (private)
//
// Push an already-constructed behaviour onto the changeable stack. A behaviour that already reports
// FINISHED is never stacked — it is queued for deletion and the result reflects whether it finished
// without failure. Otherwise the current top is told it left the stack top, a non-sub behaviour
// deactivates the current major, and the behaviour is added to the changeable stack.
int aiBEST::PushBehavior(bhvBEHAV *toPush)
{
    if (!toPush)
    {
        _apLog("~AI,Error~%s: internal error - requested push of NULL.", pBrain->names.nameInst);
        return 0;
    }

    unsigned int val = toPush->state.val;
    if (val & BHV_ST_FINISHED)
    {
        int failed = (val & BHV_ST_FAILED) != 0;
        AddToDel(&toPush);
        return failed == 0;
    }

    bhvBEHAV *top = GetTopBehavior();
    if (top)
        top->NoticeLeftStackTop();

    if ((toPush->state.val & BHV_ST_SUBBEHAV) == 0)
        InactivateMajor();

    return AddToChangeable(toPush);
}
