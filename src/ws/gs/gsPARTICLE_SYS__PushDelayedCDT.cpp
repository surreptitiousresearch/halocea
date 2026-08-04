#include "../../headers/ws/gs/gsPARTICLE_SYS.h"
#include "../../headers/ws/gs/gsPARTICLE_SYS_boundary.h"

// 0x82677AF0 — kick the global delayed collision-detection job. Clamp the pending request id to
// 0xFFFF, resize the shared result vector to it, choose a worker-thread count from the frame
// multi-thread config, then start the job. `this` is unused (operates over file-scope globals).
void gsPARTICLE_SYS::PushDelayedCDT()
{
    osPIXBeginEvent("PushDelayedCDT");

    int pendingCount = partCdtCurId;
    if ((unsigned int)partCdtCurId > 0xFFFF) {
        pendingCount = 0xFFFF;
        partCdtCurId = 0xFFFF;
    }

    partCdtResults.Resize(pendingCount);

    int threadCount;
    if (gsMsgMTConfig == 2)
        threadCount = 4;
    else if (gsMsgMTConfig == 4)
        threadCount = 32;
    else
        threadCount = 1;

    partJobCdt.Start(threadCount);

    osPIXEndEvent();
}
