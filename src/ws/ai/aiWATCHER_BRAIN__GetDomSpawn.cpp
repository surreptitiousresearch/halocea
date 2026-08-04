#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetDomSpawn @ 0x831700E8
// ?GetDomSpawn@aiWATCHER_BRAIN@@QBAPAVgsDOM_SPAWN@@XZ
gsDOM_SPAWN *aiWATCHER_BRAIN::GetDomSpawn() const
{
    if (!this->IsValid())
        return nullptr;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    return pBrain->pDomSpawn; // entENTITY base member @0x154
}
