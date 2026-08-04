#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiFILT_ENEMY::GetFirst @ 0x83220BB0
// ?GetFirst@aiFILT_ENEMY@@QAAPAVentENTITY@@XZ
//
// The entity of the first candidate record, or null when the set is empty.
entENTITY *aiFILT_ENEMY::GetFirst()
{
    if (!arr.nElem)
        return nullptr;

    aiFEE &fee = arr[0];
    return fee.hold->watcher->GetEntity();
}
