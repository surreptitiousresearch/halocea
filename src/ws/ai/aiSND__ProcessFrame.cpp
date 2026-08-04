#include "../../headers/ws/ai/aiSND.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiSND::ProcessFrame @ 0x832999F8
// ?ProcessFrame@aiSND@@UAAXXZ
//
// Per-frame sound update: run the pitch response, timed by a named profiling counter.
void aiSND::ProcessFrame()
{
    // Named per-call-site profiling counter (Meyers singleton — the original guards a file-scope
    // global with a one-time flag and an atexit destructor, which a function-local static matches).
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/b-sound", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    ProcessPitch();
}
