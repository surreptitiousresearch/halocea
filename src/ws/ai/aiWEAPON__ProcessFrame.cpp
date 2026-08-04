#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiWEAPON::ProcessFrame @ 0x83296668
// ?ProcessFrame@aiWEAPON@@UAAXM@Z
//
// Per-frame update of the brain's weapon module. The whole body is wrapped by a named profiling
// counter. For a live brain: refresh the weapon variables, trajectory checker, and current-strike
// state, age the module timers, then resolve the firing target. A dead brain does nothing but pay
// the (empty) timed scope.
void aiWEAPON::ProcessFrame(float dt)
{
    // Named per-call-site profiling counter (Meyers singleton — the original guards a file-scope
    // global with a one-time flag and an atexit destructor, which a function-local static matches).
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/aft-wpn", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    if (!pBrain->IsDead()) {
        UpdateVars();
        UpdateTraj();
        UpdateCurStrike();
        UpdateTimers(dt);
        SetupTarget();
    }
}
