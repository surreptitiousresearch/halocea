// aiWATCHER::IsInHighHO @ 0x8326D418
// ?IsInHighHO@aiWATCHER@@QBA_NXZ
#include "../../headers/ws/ai/aiWATCHER.h"

// In the high hideout: hideout mode is active but the body is not in the low hideout slot.
bool aiWATCHER::IsInHighHO() const
{
    if (!IsHideoutMode())
        return false;
    if (IsInLowHO())
        return false;
    return true;
}
