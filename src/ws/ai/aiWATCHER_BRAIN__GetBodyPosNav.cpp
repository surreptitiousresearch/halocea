#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetBodyPosNav @ 0x8316F160
// ?GetBodyPosNav@aiWATCHER_BRAIN@@UBA?AUm3dV@@XZ
// Virtual const: forward the tracked brain's nav-space body position; zero vector when the weak
// brain ref is no longer valid.
m3dV aiWATCHER_BRAIN::GetBodyPosNav() const
{
    if (!IsValid())
        return m3dVZero;
    return brain.GetPtr()->GetBodyPosNav();
}
