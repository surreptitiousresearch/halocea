#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSELENE_BASE.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"

extern int m3dUpdateTimeField(float *field, float dt); // ?m3dUpdateTimeField@@YAHPAMM@Z

// aiMIND::UpdateTimers @ 0x83228038
// ?UpdateTimers@aiMIND@@QAAXM@Z
//
// Count down the delayed-alert timer (only while it is armed, i.e. not sitting at the -1 sentinel)
// and forward `time` to every timed mind module.
void aiMIND::UpdateTimers(float time)
{
    // Advance the delayed-alert countdown unless it is parked at the -1.0 "disarmed" sentinel.
    const float fromSentinel = -1.0f - timerDelayedAlert;
    if ((fromSentinel < 0.0f ? -fromSentinel : fromSentinel) >= 0.000001f)
        m3dUpdateTimeField(&timerDelayedAlert, time);

    goal.pointee->UpdateTimers(time);
    vision.pointee->UpdateTimers(time);
    perc.pointee->UpdateTimers(time);
    enemy.pointee->UpdateTimers(time);
    best.pointee->UpdateTimers(time);
    selene.pointee->UpdateTimers(time);
    task.pointee->UpdateTimers(time);
}
