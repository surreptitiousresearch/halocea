#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/prop/propGAME_INFO.h"

// aiWATCHER_ENT::GetHeight @ 0x8328DF98
// ?GetHeight@aiWATCHER_ENT@@UBAMXZ
// Virtual const: the tracked game-info's full body height, or 0 when the watcher is invalid or has
// no game-info block.
float aiWATCHER_ENT::GetHeight() const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this); // IsValid is non-const on the base

    if (self->IsValid() && pGameInfo)
        return pGameInfo->GetHeight();
    return 0.0f;
}
