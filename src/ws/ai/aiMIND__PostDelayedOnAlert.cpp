#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"

extern m3dRND_GEN m3dRndGen; // ?m3dRndGen@@3Vm3dRND_GEN@@A — shared RNG

// aiMIND::PostDelayedOnAlert @ 0x832282C0
// ?PostDelayedOnAlert@aiMIND@@QAAXW4AI_ALERT@@_N@Z
//
// Queue `alert` to fire after a short delay. With isUseDelay the delay is a random value in the
// brain's perception [delayOnAlertMin, delayOnAlertMax] range; otherwise it is the fixed 0.5s base.
void aiMIND::PostDelayedOnAlert(AI_ALERT alert, bool isUseDelay)
{
    alertDelayed = alert;

    float delay;
    if (isUseDelay)
    {
        aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
        const aiSTATUS::PERC &perc = brain->st.pointee->perc;
        delay = m3dRndGen.RndFloatRange(perc.delayOnAlertMin, perc.delayOnAlertMax) + 0.5f;
    }
    else
    {
        delay = 0.5f;
    }
    timerDelayedAlert = delay;
}
