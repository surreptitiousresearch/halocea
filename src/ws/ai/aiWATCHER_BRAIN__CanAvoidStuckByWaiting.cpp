#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiWATCHER_BRAIN::CanAvoidStuckByWaiting @ 0x8316FC80
// ?CanAvoidStuckByWaiting@aiWATCHER_BRAIN@@UBA_NH@Z
// This unit yields (waits) instead of pushing when: it is essentially stationary, it holds the
// higher unique id (deterministic tie-break), and it is currently pathing.
bool aiWATCHER_BRAIN::CanAvoidStuckByWaiting(int uidOther) const
{
    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;

    m3dV speed;
    pBrain->GetSpeed(&speed);
    if (m3dLengthVector_2(&speed) < 1.0f)
        return false;

    if (pBrain->uniqueID <= uidOther)
        return false;

    if (!pBrain->mind.best.pointee->HasBHV(AI_BID_PATH))
        return false;

    return true;
}
