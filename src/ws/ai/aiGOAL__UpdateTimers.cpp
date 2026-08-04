// aiGOAL::UpdateTimers @ 0x832AF1A8
// ?UpdateTimers@aiGOAL@@UAAXM@Z
#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// Advance this module's roam-change and stuck-event countdown timers by `dt` seconds.
void aiGOAL::UpdateTimers(float dt)
{
    m3dUpdateTimeField(&timerChangeRoam, dt);
    m3dUpdateTimeField(&timerStuckEvent, dt);
}
