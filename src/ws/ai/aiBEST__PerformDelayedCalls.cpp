#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiBEST::PerformDelayedCalls @ 0x832978D0
// ?PerformDelayedCalls@aiBEST@@AAAXXZ
//
// Run the best-behaviour module's deferred per-frame calls (currently just the pending grenade
// notice), timed under the "b-best-dc" profiling counter.
void aiBEST::PerformDelayedCalls()
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/b-best-dc", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    PreformNoticeGrenade();
}
