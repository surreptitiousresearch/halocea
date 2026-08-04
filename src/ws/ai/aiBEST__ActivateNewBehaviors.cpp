#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBEST::ActivateNewBehaviors @ 0x832973F0
// ?ActivateNewBehaviors@aiBEST@@AAAXXZ
//
// Repeatedly notify the topmost not-yet-activated behaviour (re-flattening the execution stack each
// pass) until every changeable behaviour is activated or the 30-pass safety bound is hit. Diagnoses
// the failure cases: a leftover non-activated behaviour mid-stack, a null stack after an
// activation, or an activation "tie" that could not be broken (which empties the stack).
void aiBEST::ActivateNewBehaviors()
{
    FillExecStack();
    if (!arrChangeable.nEntry)
        return;

    bhvBEHAV *last = nullptr;
    int i;
    for (i = 30; i > 0; --i) {
        if (arrExecutable.nEntry <= 0)
            break;
        last = FindNonActivated();
        if (!last)
            break;
        SendTopNotifications(last);
        FillExecStack();
    }

    if (arrChangeable.nEntry) {
        if (i > 0) {
            bhvBEHAV *stray = FindNonActivated();
            if (stray)
                _apLog("~AI,Error~%s: non-activated '%s' in the middle of the stack.",
                       pBrain->names.nameInst, stray->SLID);
        } else {
            _apLog("~AI,Error~%s: broke tie of '%s' activation - emptying the stack",
                   pBrain->names.nameInst, last->SLID);
            EraseAll(BHV_ST_MANUALLY_CANCELLED);
        }
    } else {
        _apLog("~AI,Error~%s: internal error - null stack after '%s' activation.",
               pBrain->names.nameInst, last->SLID);
    }
}
