#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiBEST::CommonPFBeforeBody @ 0x832977E8
// ?CommonPFBeforeBody@aiBEST@@AAAXP8bhvBEHAV@@AAXXZ@Z
//
// Shared pre-body per-frame step: clear the per-frame mind/weapon status bits, commit pending stack
// changes and ensure the stack is non-empty, then (timed) run the given behaviour hook `proc` on the
// current top behaviour unless it has finished. `proc` is a pointer-to-member of bhvBEHAV (the
// fast/full pre-body update selected by the caller).
void aiBEST::CommonPFBeforeBody(void (bhvBEHAV::*proc)())
{
    aiSTATUS *status = pBrain->st.pointee;
    status->mind.val &= 0xFFF5FFFF;
    status->wpn.val &= ~0x10;

    CommitChanges();
    EnsureNonEmpty();

    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/b-best-pfb", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    bhvBEHAV *top = GetTopBehavior();
    if (top && (top->state.val & BHV_ST_FINISHED) == 0)
        (top->*proc)();
}
