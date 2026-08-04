// aiGOAL::UpdatePosReached @ 0x832AF488
// ?UpdatePosReached@aiGOAL@@IAAX_N@Z
#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// Set/clear the status "position reached" body flag (bit 0x10) from the reach test against the
// command check-reach point, accumulating time-at-goal while reached (resetting it otherwise).
void aiGOAL::UpdatePosReached(bool use2DNavigation)
{
    aiBRAIN *brain = pBrain;
    aiSTATUS *st = brain->st.pointee;
    bool reached = IsPointClose(brain->cmd.goal.posChkReach, use2DNavigation);
    int flags = st->body.val;
    if (reached)
    {
        st->body.val = flags | 0x10;
        st->goal.timePosReached = pBrain->GetElapsedTime() + st->goal.timePosReached;
    }
    else
    {
        st->body.val = flags & ~0x10;
        st->goal.timePosReached = 0.0f;
    }
}
