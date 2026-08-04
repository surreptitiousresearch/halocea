#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsShooting @ 0x8316FD30
// ?IsShooting@aiWATCHER_BRAIN@@UBA_NXZ
bool aiWATCHER_BRAIN::IsShooting() const
{
    if (!this->IsValid())
        return false;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    if (!pBrain->body.pointee)
        return false;
    if (!pBrain->body.pointee->wpn.pointee)
        return false;

    return pBrain->body.pointee->wpn.pointee->IsShootingNow();
}
