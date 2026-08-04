#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/ds/WEAK_PTR.h"

// aiWEAPON::GetCtrl @ 0x83294920
// ?GetCtrl@aiWEAPON@@IBAPAVpropWPN_AI_USABLE@@ABV?$aiW_IDX@VpropWPN_AI_USABLE@@@@@Z
//
// Resolve a controller index into the live controller it names, or null when the index is out of
// range or the referenced controller has died.
propWPN_AI_USABLE *aiWEAPON::GetCtrl(const aiW_IDX<propWPN_AI_USABLE> &idx) const
{
    int i = idx.idx;
    if (i >= 0 && i < ctrlsList.nElem) {
        const CTRL_INFO &info = ctrlsList[i];
        if (info.ctrl.pHandle)
            return info.ctrl.pHandle->pPtr;
    }
    return nullptr;
}
