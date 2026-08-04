#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsEnemyVisibleNow @ 0x8316FF00
// ?IsEnemyVisibleNow@aiWATCHER_BRAIN@@QBA_NXZ
bool aiWATCHER_BRAIN::IsEnemyVisibleNow() const
{
    if (!this->IsValid())
        return false;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    // aiSTATUS::est bit 0x10 = "enemy visible now"
    return ((unsigned int)pBrain->st.pointee->est.val >> 4) & 1;
}
