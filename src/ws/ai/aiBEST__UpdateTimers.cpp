#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiBEST::UpdateTimers @ 0x83296890
// ?UpdateTimers@aiBEST@@UAAXM@Z
//
// Age every behaviour on the active stack (top to bottom, walking parent links) by `dt` seconds,
// then advance the parcel-storage timers.
void aiBEST::UpdateTimers(float dt)
{
    for (bhvBEHAV *b = GetTopBehavior(); b; b = GetParentBehavior(b))
        b->UpdateTimers(dt);

    parcelStorage.UpdateTime(dt);
}
