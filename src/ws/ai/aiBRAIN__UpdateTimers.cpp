#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiTIMER_POOL.h"

// aiBRAIN::UpdateTimers() @ 0x832AEF78
// ?UpdateTimers@aiBRAIN@@UAAXXZ
//
// Advance every timer this frame by the actor's elapsed time: the mind timers, the countdown timer
// pool, then dispatch any pool timers that just fired.
void aiBRAIN::UpdateTimers()
{
    float elapsed = GetElapsedTime();
    mind.UpdateTimers(elapsed);
    timers.Update(elapsed);
    ProcessTimerPool();
}
