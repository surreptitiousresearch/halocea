#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsIdle @ 0x8316FE00
// ?IsIdle@aiWATCHER_BRAIN@@UBA_NXZ
bool aiWATCHER_BRAIN::IsIdle() const
{
    if (!this->IsValid())
        return true;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    return pBrain->mind.IsIdle();
}
