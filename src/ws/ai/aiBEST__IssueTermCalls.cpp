#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A

// aiBEST::IssueTermCalls @ 0x83297FC0
// ?IssueTermCalls@aiBEST@@AAAXXZ
//
// Run the terminate sequence on every behaviour queued for deletion that has not yet done it
// (BHV_ST_DONE_TERM_SEQ clear): cancel it, notify it it left the stack top, drop its timers, term
// it, and mark the terminate sequence done. The loop re-reads nEntry each pass because the callees
// may append to the to-delete list.
void aiBEST::IssueTermCalls()
{
    for (int idx = 0; idx < arrToDel.nEntry; ++idx) {
        if (!IGNORE_STRONG_ASSERT && ((unsigned int)idx >= 0x14 || idx >= arrToDel.nEntry))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);

        bhvBEHAV *cur = arrToDel.list[idx];
        if (cur && (cur->state.val & BHV_ST_DONE_TERM_SEQ) == 0) {
            cur->Cancel();
            cur->NoticeLeftStackTop();
            pBrain->RemoveTimersByContext(cur->bhvSNum);
            cur->Term();
            cur->state.val |= BHV_ST_DONE_TERM_SEQ;
        }
    }
}
