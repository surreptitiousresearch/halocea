#include "../../headers/ws/ai/aiBRAIN.h"

// aiMIND::ForceDelayedCalls @ 0x83227D30
// ?ForceDelayedCalls@aiMIND@@QAAXXZ
//
// If a delayed alert is still pending (timer positive), fire it on the brain immediately and
// disarm the timer.
void aiMIND::ForceDelayedCalls()
{
    if (timerDelayedAlert > 0.000001f)
    {
        aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
        brain->OnAlert(alertDelayed);
        timerDelayedAlert = -1.0f;
    }
}
