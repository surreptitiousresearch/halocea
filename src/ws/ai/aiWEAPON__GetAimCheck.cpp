#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE_DESC.h"
#include "../../headers/ws/prop/propBASE.h"

// aiWEAPON::GetAimCheck @ 0x83294FE0
// ?GetAimCheck@aiWEAPON@@IBA?AW4aimCHECK_TYPE@@ABV?$aiW_IDX@VpropWPN_AI_USABLE@@@@@Z
//
// The aim-check mode for the controller at `idx`, taken from its descriptor. Falls back to mode 1
// (body-relative) when the index is out of range or the controller slot is empty.
aimCHECK_TYPE aiWEAPON::GetAimCheck(const aiW_IDX<propWPN_AI_USABLE> &idx) const
{
    int i = idx.idx;
    if (i >= 0 && i < ctrlsList.nElem) {
        const CTRL_INFO &info = ctrlsList[i];
        propWPN_AI_USABLE *ctrl = info.ctrl.pHandle ? info.ctrl.pHandle->pPtr : nullptr;
        if (ctrl) {
            const propWPN_AI_USABLE_DESC *desc =
                (const propWPN_AI_USABLE_DESC *)((const propBASE *)ctrl)->spDesc.pointee;
            return desc->typeIsAimed;
        }
    }
    return (aimCHECK_TYPE)1;
}
