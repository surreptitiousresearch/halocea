#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;          // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;       // shared empty-string / assert-info byte @0x8200155A
extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBEST::PopBehavior @ 0x83296DF0
// ?PopBehavior@aiBEST@@AAAXPAVbhvBEHAV@@@Z
//
// Pop `pBehav` (which must be the current top) off the changeable stack, together with any finished
// behaviours immediately below it. Each popped behaviour is issued an end-of-body command (until a
// non-sub-behaviour is reached), queued for deletion, and cleared from the slot. Afterwards the
// body-responsible major is restored, terminate calls are issued, the new top is re-notified, and
// all sparsings are reset. If `pBehav` is not the top, it is a no-op (a diagnostic is logged only
// when the behaviour cannot be found on either the to-delete or the changeable stack).
void aiBEST::PopBehavior(bhvBEHAV *pBehav)
{
    int nEntry = arrChangeable.nEntry;
    if (nEntry <= 0)
        return;

    if (pBehav == arrChangeable[nEntry - 1]) {
        int majorResponsibleForBody = FindMajorResponsibleForBody();
        bool doBodyCmd = true;
        int idx = nEntry - 1;
        do {
            bhvBEHAV *cur = arrChangeable[idx];
            if (doBodyCmd) {
                cur->IssueBodyCmd_ToEnd();
                if ((cur->state.val & BHV_ST_SUBBEHAV) == 0)
                    doBodyCmd = false;
            }
            AddToDel(&cur);
            arrChangeable[idx] = nullptr;
            if (!IGNORE_STRONG_ASSERT && (unsigned int)idx > 0xA)
                STRONG_ASSERT_DUMMY::Crash(nullptr, "n <= size && n >= 0",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 313, byte_8200155A);
            arrChangeable.nEntry = idx;
            --idx;
        } while (idx >= 0 && (arrChangeable[idx]->state.val & BHV_ST_FINISHED) != 0);

        BeginBodyResponsibleMajor(majorResponsibleForBody);
        IssueTermCalls();
        if (arrChangeable.nEntry > 0)
            SendTopNotifications(arrChangeable[arrChangeable.nEntry - 1]);
        pBrain->ResetAllSparsings();
    } else {
        // Not the top: only diagnose when it is on neither the to-delete nor the changeable stack.
        bool found = false;
        for (int i = 0; i < arrToDel.nEntry; ++i)
            if (arrToDel.list[i] == pBehav) { found = true; break; }
        if (!found) {
            for (int i = 0; i < arrChangeable.nEntry; ++i)
                if (arrChangeable.list[i] == pBehav) { found = true; break; }
            if (!found)
                _apLog("~AI,Error~%s: can't find behavior '%s', ignoring.",
                       pBrain->names.nameInst, pBehav->SLID);
        }
    }
}
