#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON::ActivateCtrl @ 0x83294D50
// ?ActivateCtrl@aiWEAPON@@IAAXPBVpropWPN_AI_USABLE@@_N@Z
//
// Find the registered slot for controller `ctrl` (matching by resolved weak-ref pointer; a null
// `ctrl` matches an empty slot) and set its active flag to `isActivate`.
void aiWEAPON::ActivateCtrl(const propWPN_AI_USABLE *ctrl, bool isActivate)
{
    int n = ctrlsList.nElem;
    for (int i = 0; i < n; ++i) {
        CTRL_INFO &info = ctrlsList[i];
        const propWPN_AI_USABLE *slotCtrl = info.ctrl.pHandle ? info.ctrl.pHandle->pPtr : nullptr;
        if (slotCtrl == ctrl) {
            info.isActive = isActivate;
            return;
        }
    }
}
