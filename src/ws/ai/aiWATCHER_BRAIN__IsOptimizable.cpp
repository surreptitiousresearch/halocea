#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsOptimizable @ 0x8316FE80
// ?IsOptimizable@aiWATCHER_BRAIN@@QBA_NXZ
bool aiWATCHER_BRAIN::IsOptimizable() const
{
    if (!this->IsValid())
        return false;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    // aiSTATUS::gen bit 0x10 = "optimizable"
    return ((unsigned int)pBrain->st.pointee->gen.val >> 4) & 1;
}
