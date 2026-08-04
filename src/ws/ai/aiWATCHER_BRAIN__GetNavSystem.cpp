#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetNavSystem @ 0x8316F9F8
// ?GetNavSystem@aiWATCHER_BRAIN@@QBAPAVnavSYS@@XZ
// Forward to the tracked brain's owning nav system; null when the weak brain ref is no longer valid.
navSYS *aiWATCHER_BRAIN::GetNavSystem() const
{
    if (!IsValid())
        return nullptr;
    return brain.GetPtr()->GetNavSystem();
}
