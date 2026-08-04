#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"

// aiWEAPON::IsCtrlActive @ 0x83294C78
// ?IsCtrlActive@aiWEAPON@@IBA_NPBVwpnWEAPON@@@Z
//
// Is the controller driving weapon interface `iface` currently active? Scan the registered
// controller slots for the one whose resolved interface matches `iface`, then report whether that
// slot is both active and enabled by the custom-weapon gate.
bool aiWEAPON::IsCtrlActive(const wpnWEAPON *iface) const
{
    if (!iface)
        return false;

    int n = ctrlsList.nElem;
    for (int i = 0; i < n; ++i) {
        const CTRL_INFO &info = ctrlsList[i];
        ds::WEAK_PTR_HANDLE<propWPN_AI_USABLE> *pHandle = info.ctrl.pHandle;
        if (pHandle && pHandle->pPtr != nullptr) {
            // Copy the weak-ref (bumps the handle refcount, as the binary does inline) so GetIFace
            // resolves it safely.
            ds::WEAK_PTR<propWPN_AI_USABLE> tmp(info.ctrl);
            if (GetIFace(tmp) == iface)
                return info.isActive && info.isEnabledByCustomWeapon;
        }
    }
    return false;
}
