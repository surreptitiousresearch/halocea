// aiMIND::ExecuteDelayedOnAlert @ 0x8322BCB0
// ?ExecuteDelayedOnAlert@aiMIND@@IAAXXZ  (protected)
//
// Fire a pending delayed alert the moment its countdown reaches (but has not been disarmed past)
// zero: the timer is armed with a positive value, ticks down toward 0, and is disarmed to the
// sentinel -1. This fires only in the window where the timer is <= ~0 but not yet the -1 sentinel.
#include "../../headers/ws/ai/aiBRAIN.h"

void aiMIND::ExecuteDelayedOnAlert()
{
    // distance from the -1 "disarmed" sentinel (fires only when NOT already disarmed)
    float distFromDisarmed = -1.0f - timerDelayedAlert;
    if (distFromDisarmed < 0.0f)
        distFromDisarmed = -distFromDisarmed;

    if (distFromDisarmed >= 0.000001f && timerDelayedAlert <= 0.000001f)
    {
        aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
        brain->OnAlert(alertDelayed);
        timerDelayedAlert = -1.0f; // disarm
        ZeroISA();
    }
}
