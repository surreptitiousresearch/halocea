#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/prop/propGAME_INFO.h"

// aiWATCHER_ENT::GetMinHeightFlesh @ 0x8328DF18
// ?GetMinHeightFlesh@aiWATCHER_ENT@@UBAMXZ
// Virtual const: the tracked game-info's minimum flesh height, or 0 when the watcher is invalid or
// has no game-info block.
float aiWATCHER_ENT::GetMinHeightFlesh() const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this); // IsValid is non-const on the base

    if (self->IsValid() && pGameInfo)
        return pGameInfo->GetMinHeightFlesh();
    return 0.0f;
}
