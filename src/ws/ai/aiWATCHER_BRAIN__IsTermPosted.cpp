#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsTermPosted @ 0x8316FA80
// ?IsTermPosted@aiWATCHER_BRAIN@@QBA_NXZ
bool aiWATCHER_BRAIN::IsTermPosted() const
{
    if (!this->IsValid())
        return true;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    // msgADDR::stateAddr bit 0x8 = "terminate posted"
    return (pBrain->stateAddr.state >> 3) & 1;
}
