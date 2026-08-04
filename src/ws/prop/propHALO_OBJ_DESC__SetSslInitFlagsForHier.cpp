#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propHALO_OBJ_DESC::SetSslInitFlagsForHier(const dsTYPE_ID&) @ 0x823EB380
// Mark the lazy ssl-class-init flag done when the hierarchy-wide `typeId` being propagated
// matches this class's own TYPE_ID exactly.
void propHALO_OBJ_DESC::SetSslInitFlagsForHier(const dsTYPE_ID &typeId)
{
    if (propHALO_OBJ_DESC::TYPE_ID.id == typeId.id)
        this->isEntSslClassInited.val = true;
}
