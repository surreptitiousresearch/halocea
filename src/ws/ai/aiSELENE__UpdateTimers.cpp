#include "../../headers/ws/ai/aiSELENE.h"

// m3dUpdateTimeField — count `*field` down by `dt`, clamping at 0; returns whether it hit 0 this
// call (?m3dUpdateTimeField@@YAHPAMM@Z). boundary.
extern int m3dUpdateTimeField(float *field, float dt);

// aiSELENE::UpdateTimers() @ 0x832B39D0
// ?UpdateTimers@aiSELENE@@UAAXM@Z
//
// Age the enemy-selection timers by `dt`: the "select best enemy" cadence and the "wait for the
// find-enemy CDT result" window.
void aiSELENE::UpdateTimers(float dt)
{
    m3dUpdateTimeField(&timerSelectBestEnemy, dt);
    m3dUpdateTimeField(&timerWaitCDTResult, dt);
}
