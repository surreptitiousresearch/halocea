#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiWATCHER_PLAYER::GetUID @ 0x8328ED48
// ?GetUID@aiWATCHER_PLAYER@@UBAKXZ
// Virtual const: the tracked player entity's stable message-system unique id. Returns 0xFFFFFFFF
// (-1) when the weak ref is no longer valid.
unsigned int aiWATCHER_PLAYER::GetUID() const
{
    aiWATCHER_PLAYER *self = const_cast<aiWATCHER_PLAYER *>(this);
    if (!self->IsValid())
        return 0xFFFFFFFFu;
    entENTITY *ent = player.pHandle ? player.pHandle->pPtr : nullptr;
    return (unsigned int)ent->uniqueID;
}
