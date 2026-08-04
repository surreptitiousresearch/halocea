#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::MarkToReduceVision @ 0x8316F990
// ?MarkToReduceVision@aiWATCHER_BRAIN@@QAAXXZ
// Request that the tracked brain's perception module shrink its vision cone on the next tick.
// No-op when the weak brain ref is no longer valid.
void aiWATCHER_BRAIN::MarkToReduceVision()
{
    if (IsValid())
        brain.GetPtr()->st.pointee->perc.needToReduceVision = true;
}
