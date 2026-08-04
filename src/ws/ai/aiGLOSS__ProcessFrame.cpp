#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/ai/aiDEBUG.h" // IsOffAI / HasMovingWatchee

extern int m3dUpdateTimeField(float *field, float dt); // count *field down by dt, clamp at 0

// aiGLOSS::ProcessFrame @ 0x8323A3E8
// ?ProcessFrame@aiGLOSS@@QAAXM@Z
//
// Per-frame tick: age every per-shooter sparsing cooldown timer by `delta`. Skipped entirely while
// AI is switched off unless a debug watchee is currently moving.
void aiGLOSS::ProcessFrame(float delta)
{
    if (aiDEBUG::IsOffAI() && !aiDEBUG::HasMovingWatchee())
        return;

    int count = sparsings.nElem;
    for (int i = 0; i < count; ++i)
        m3dUpdateTimeField(&sparsings.pData[i].timer, delta);
}
