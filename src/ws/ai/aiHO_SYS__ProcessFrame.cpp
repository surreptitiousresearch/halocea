#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiHO_SYS::ProcessFrame @ 0x832A6C50
// ?ProcessFrame@aiHO_SYS@@UAAXM@Z
//
// Per-frame HO update (timed). Counts down `framesToLeaveHO`; when it reaches zero, process the
// pending leave, then decrement (so it goes inactive at -1). `dt` is unused by this build.
void aiHO_SYS::ProcessFrame(float dt)
{
    (void)dt;
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/aft-hosys", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    if (framesToLeaveHO >= 0) {
        if (framesToLeaveHO == 0)
            ProcessLeaveHO();
        --framesToLeaveHO;
    }
}
