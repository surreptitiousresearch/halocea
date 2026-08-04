#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiFILT_ENEMY::GetAny() @ 0x83220B50
// ?GetAny@aiFILT_ENEMY@@QAAPAVentENTITY@@XZ
// The first surviving FEE's entity, or null if the working set is empty.
entENTITY *aiFILT_ENEMY::GetAny()
{
    if (!arr.nElem)
        return nullptr;

    aiFEE &fee = arr[0];
    return fee.hold->watcher->GetEntity();
}
