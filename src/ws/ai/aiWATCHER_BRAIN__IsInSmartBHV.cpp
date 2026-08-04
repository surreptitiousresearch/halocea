#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsInSmartBHV @ 0x8316FF80
// ?IsInSmartBHV@aiWATCHER_BRAIN@@QBA_NXZ
bool aiWATCHER_BRAIN::IsInSmartBHV() const
{
    if (!this->IsValid())
        return false;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    if (pBrain->mind.best.pointee->HasBHV(AI_BID_SMART))
        return true;

    aiBRAIN *pBrainMajor = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    if (pBrainMajor->mind.best.pointee->HasBHV(AI_BID_MAJOR_SMART))
        return true;

    return false;
}
