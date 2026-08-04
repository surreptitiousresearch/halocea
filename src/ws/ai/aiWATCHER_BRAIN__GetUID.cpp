#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetUID @ 0x8316F7E0
// ?GetUID@aiWATCHER_BRAIN@@UBAKXZ
// Virtual const: the tracked brain's stable message-system unique id (msgADDR::uniqueID). Returns
// 0xFFFFFFFF (-1) when the weak brain ref is no longer valid.
unsigned int aiWATCHER_BRAIN::GetUID() const
{
    if (!IsValid())
        return 0xFFFFFFFFu;
    return (unsigned int)brain.GetPtr()->uniqueID;
}
