#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetWannaBePos @ 0x8316F8E8
// ?GetWannaBePos@aiWATCHER_BRAIN@@QAAXAAUm3dV@@@Z
// Write the position the brain's "best behaviour" module currently wants to reach into `pos`.
// Leaves the undefined-vector sentinel when the weak brain ref is no longer valid.
void aiWATCHER_BRAIN::GetWannaBePos(m3dV &pos)
{
    if (!IsValid())
    {
        pos = m3dVUndef;
        return;
    }
    brain.GetPtr()->mind.best.pointee->GetWannaBePos(&pos);
}
