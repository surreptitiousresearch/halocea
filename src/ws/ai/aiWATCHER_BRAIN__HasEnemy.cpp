#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::HasEnemy @ 0x83170060
// ?HasEnemy@aiWATCHER_BRAIN@@QBA_NPBVentENTITY@@@Z
bool aiWATCHER_BRAIN::HasEnemy(const entENTITY *ent) const
{
    if (!this->IsValid())
        return false;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    return pBrain->mind.enemy.pointee->Is(ent);
}
