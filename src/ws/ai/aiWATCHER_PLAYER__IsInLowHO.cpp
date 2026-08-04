#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"

// aiWATCHER_PLAYER::IsInLowHO @ 0x8328E750
// ?IsInLowHO@aiWATCHER_PLAYER@@UBA_NXZ
// Virtual const: the player occupies the low hideout slot. In wall-lean mode this defers to the
// generic hideout-mode test; otherwise it reads the packed AI_FSM_PROPS low-HO bit (bit 12).
bool aiWATCHER_PLAYER::IsInLowHO() const
{
    aiWATCHER *self = const_cast<aiWATCHER *>(static_cast<const aiWATCHER *>(this)); // IsValid is non-const

    if (!self->IsValid())
        return false;

    if (IsWlnMode())
        return self->IsHideoutMode();

    return (fsmPropFlags.val >> 12) & 1;
}
