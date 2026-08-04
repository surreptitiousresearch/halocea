#include "../../headers/ws/ai/aiBEST.h"

// aiBEST::CommonPFAfterBody @ 0x83297738
// ?CommonPFAfterBody@aiBEST@@AAAXP8bhvBEHAV@@AAXXZ@Z
//
// Shared post-body per-frame step: run the given behaviour hook `proc` on the current top behaviour
// (unless it finished), pop it if it both finished and is still the changeable-stack top, then
// commit all pending stack changes. `proc` is a pointer-to-member of bhvBEHAV (the fast/full
// post-body update selected by the caller).
void aiBEST::CommonPFAfterBody(void (bhvBEHAV::*proc)())
{
    bhvBEHAV *top = GetTopBehavior();
    if (top) {
        if ((top->state.val & BHV_ST_FINISHED) == 0)
            (top->*proc)();

        int idx = arrChangeable.nEntry - 1;
        bhvBEHAV *stackTop = (idx < 0) ? nullptr : arrChangeable[idx];
        if (top == stackTop && (top->state.val & BHV_ST_FINISHED) != 0)
            PopBehavior(top);
    }
    CommitChanges();
}
