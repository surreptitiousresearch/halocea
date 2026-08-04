#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetBodyCS @ 0x8316F2D0
// ?GetBodyCS@aiWATCHER_BRAIN@@UBAXAAUm3dV@@00@Z
// Virtual const: forward the tracked brain's body coordinate system (position + forward + up).
// Falls back to (origin, +Z, +Y) when the weak brain ref is no longer valid.
void aiWATCHER_BRAIN::GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp) const
{
    if (!IsValid())
    {
        pos   = m3dVZero;
        dir   = m3dVUnitZ;
        dirUp = m3dVUnitY;
        return;
    }
    aiBRAIN *b = brain.GetPtr();
    pos   = b->GetPos();
    dir   = b->GetDir();
    dirUp = b->GetDirUp();
}
