#include "../../headers/ws/ai/aiBEST.h"

// aiBEST::ProcessFrameBeforeBody @ 0x83297A10
// ?ProcessFrameBeforeBody@aiBEST@@UAAXXZ  (virtual override)
//
// Pre-body per-frame step (full path): perform deferred calls, run the shared before-body update
// driving each behaviour's ProcessFrameBeforeBody hook, then (unless background updates are disabled
// by state bit BEST_ST_BACKGROUND_DISABLED) walk from the top behaviour down through its parents,
// giving each not-yet-finished background behaviour a background-frame tick until one is no longer a
// sub-behaviour. Timing wrappers around the two phases are elided for clarity.
void aiBEST::ProcessFrameBeforeBody()
{
    PerformDelayedCalls();

    CommonPFBeforeBody(&bhvBEHAV::ProcessFrameBeforeBody);

    if ((state.val & BEST_ST_BACKGROUND_DISABLED) == 0) {
        const bhvBEHAV *top = GetTopBehavior();
        for (bhvBEHAV *bg = GetParentBehavior(top); bg; bg = GetParentBehavior(bg)) {
            if ((bg->state.val & BHV_ST_FINISHED) == 0) {
                bg->ProcessBackgroundFrame();
                if ((bg->state.val & BHV_ST_SUBBEHAV) == 0)
                    break;
            }
        }
    }
}
