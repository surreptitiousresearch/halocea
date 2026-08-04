#include "../../headers/ws/gs/gsPARTICLE_SYS.h"
#include "../../headers/ws/gs/gsPARTICLE_SYS_boundary.h"

// 0x82673A98 — stop the global delayed-CDT job on the given worker thread mask. `this` is
// unused; the work targets the single global partJobCdt instance.
void gsPARTICLE_SYS::StopDelayedCDT(int threadId)
{
    partJobCdt.Stop(threadId);
}
