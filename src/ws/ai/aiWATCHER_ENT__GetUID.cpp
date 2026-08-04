#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiWATCHER_ENT::GetUID @ 0x8328F0C0
// ?GetUID@aiWATCHER_ENT@@UBAKXZ
// Virtual const: the tracked entity's stable message-system unique id. Returns 0xFFFFFFFF (-1)
// when the weak ref is no longer valid.
unsigned int aiWATCHER_ENT::GetUID() const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this);
    if (!self->IsValid())
        return 0xFFFFFFFFu;
    entENTITY *ent = entity.pHandle ? entity.pHandle->pPtr : nullptr;
    return (unsigned int)ent->uniqueID;
}
