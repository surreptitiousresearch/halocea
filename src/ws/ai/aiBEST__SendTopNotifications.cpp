#include "../../headers/ws/ai/aiBEST.h"

// aiBEST::SendTopNotifications @ 0x83297010
// ?SendTopNotifications@aiBEST@@AAAXPAVbhvBEHAV@@@Z
//
// Bring the freshly-topmost behaviour up to date: if it is already finished, only tag it as
// notified; otherwise, on first activation issue its "become-active" body command, then its
// on-stack-top notice. If activating it finished it immediately, pop and re-fill the stack.
void aiBEST::SendTopNotifications(bhvBEHAV *pBehav)
{
    if (!pBehav)
        return;

    unsigned int val = pBehav->state.val;
    if (val & BHV_ST_FINISHED) {
        pBehav->state.val = val | BHV_ST_ACTIVATED;
        return;
    }

    if ((val & BHV_ST_ACTIVATED) == 0) {
        pBehav->state.val |= BHV_ST_ACTIVATED;
        pBehav->NoticeActive();
        if ((pBehav->state.val & BHV_ST_FINISHED) == 0)
            pBehav->IssueBodyCmd_ToBegin();
    }
    if ((pBehav->state.val & BHV_ST_FINISHED) == 0)
        pBehav->NoticeOnStackTop();
    if (pBehav->state.val & BHV_ST_FINISHED) {
        PopBehavior(pBehav);
        EnsureNonEmpty();
    }
}
