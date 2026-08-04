#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ap/apCOUNTER_CUSTOM.h"

// aiDEF_CDT_SYS::ProcessFrame @ 0x8323F188
// ?ProcessFrame@aiDEF_CDT_SYS@@UAAXM@Z
//
// Per-frame tick: publish the total/new request counters, then analyse in-flight results, promote
// this frame's accumulated requests, and age the destroy timers.
void aiDEF_CDT_SYS::ProcessFrame(float dt)
{
    static apCOUNTER_CUSTOM cnt_map_reqs(
        "process_frame/ai/brain/gs/aiDEF_CDT_SYS/ProcessFrame reqs", "total/new", nullptr);

    cnt_map_reqs.SetValue(0, (float)(dstReqs.count + srcReqs.count));
    cnt_map_reqs.SetValue(1, (float)srcReqs.count);

    AnalyzeResults();
    ProcessRequests();
    UpdateTimers(dt);
}
