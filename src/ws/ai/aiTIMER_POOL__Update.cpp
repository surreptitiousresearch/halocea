#include "../../headers/ws/ai/aiTIMER_POOL.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // m3dUpdateTimeField

// aiTIMER_POOL::Update @ 0x83179D50
// ?Update@aiTIMER_POOL@@QAAXM@Z
//
// Advance every live timer's countdown by `dt` seconds. (The mangle takes a float; the decompiler
// widened it to double.)
void aiTIMER_POOL::Update(float dt)
{
    int nElem = timers.nElem;
    for (int i = 0; i < nElem; ++i)
        m3dUpdateTimeField(&timers[i].timer, dt);
}
