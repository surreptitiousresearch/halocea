#include "../../headers/ws/ai/aiTIMER_POOL.h"

// aiTIMER_POOL::aiTIMER::CheckDone @ 0x83176E40
// ?CheckDone@aiTIMER@aiTIMER_POOL@@QAA_NXZ
//
// Report whether this timer has elapsed. The comparison against 0.000001 is the engine's
// "effectively zero" epsilon. A cycling timer auto-reloads to its default value when done.
bool aiTIMER_POOL::aiTIMER::CheckDone()
{
    bool done = timer < 0.000001f;
    if (done && isCycle)
        timer = defVal;
    return done;
}
