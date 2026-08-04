#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsDead @ 0x8316F858
// ?IsDead@aiWATCHER_BRAIN@@UBA_NXZ
// Virtual const: true when the tracked brain is dead. A no-longer-valid weak ref reports dead.
bool aiWATCHER_BRAIN::IsDead() const
{
    if (!IsValid())
        return true;
    return brain.GetPtr()->IsDead();
}
