#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiBEST::ProcessFrameAfterBody @ 0x83297960
// ?ProcessFrameAfterBody@aiBEST@@UAAXXZ  (virtual override)
//
// Post-body per-frame step (full path): run the shared after-body update driving each behaviour's
// ProcessFrameAfterBody hook, then advance and prune the parcel storage. Timed under "aft-best".
void aiBEST::ProcessFrameAfterBody()
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/aft-best", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    CommonPFAfterBody(&bhvBEHAV::ProcessFrameAfterBody);
    parcelStorage.UpdateFrame(1);
    parcelStorage.RemoveExpired();
}
