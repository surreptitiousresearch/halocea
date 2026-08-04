#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsInLowHO @ 0x8316F518
// ?IsInLowHO@aiWATCHER_BRAIN@@UBA_NXZ
// Virtual const: true when the unit is in a "low hideout" — it is in hideout mode, not wall-leaning,
// and the brain status body-state flag bit 0x1000 is set. When wall-leaning, low-HO is reported
// simply as the hideout-mode state (matches the decompiler's early return of IsHideoutMode()).
bool aiWATCHER_BRAIN::IsInLowHO() const
{
    if (!IsHideoutMode())
        return false;
    if (IsWlnMode())
        return IsHideoutMode();
    return (brain.GetPtr()->st.pointee->body.val >> 12) & 1;
}
