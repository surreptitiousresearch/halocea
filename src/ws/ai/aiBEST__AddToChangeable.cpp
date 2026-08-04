#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBEST::AddToChangeable @ 0x83298550
// ?AddToChangeable@aiBEST@@AAAHPAVbhvBEHAV@@@Z
//
// Push `pBehav` onto the changeable stack. If it has not finished (BHV_ST_FINISHED clear): push it
// (or, on stack overflow, fail it and queue it for deletion, returning 0). If it arrived already
// finished, it is queued for deletion instead — returning 0 (with an error) if it had failed to
// init (BHV_ST_FAILED), otherwise 1.
int aiBEST::AddToChangeable(bhvBEHAV *pBehav)
{
    unsigned int val = pBehav->state.val;

    if ((val & BHV_ST_FINISHED) == 0) {
        if (arrChangeable.nEntry >= 10) {
            _apLog("~AI,Error~%s: Behavior stack overflow. Shrinking.", pBrain->names.nameInst);
            pBehav->Fail();
            bhvBEHAV *toDel = pBehav;
            AddToDel(&toDel);
            return 0;
        }
        arrChangeable.Add(pBehav);
        return 1;
    }

    bool hadFailed = (val & BHV_ST_FAILED) != 0;
    dsTSTRING<char> slid(pBehav->SLID);
    bhvBEHAV *toDel = pBehav;
    AddToDel(&toDel);
    if (!hadFailed)
        return 1;

    _apLog("~AI,Error~%s: Behavior '%s' could not init.", pBrain->names.nameInst, slid.CStr());
    return 0;
}
