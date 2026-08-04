#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiWATCHER_BRAIN::FillStrikeImmune(dsSTRID_FLAGS& flags) const @ 0x83172A60
// ?FillStrikeImmune@aiWATCHER_BRAIN@@UBAXAAUdsSTRID_FLAGS@@@Z
//
// If the watcher is still valid, replace `flags` with the tracked brain descriptor's set of
// strike-immune damage types. No-op otherwise.
void aiWATCHER_BRAIN::FillStrikeImmune(dsSTRID_FLAGS &flags) const
{
    if (!IsValid())
        return;

    const aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    flags.Set(*aiBrainStrikeImmuneFlags(pBrain));
}
