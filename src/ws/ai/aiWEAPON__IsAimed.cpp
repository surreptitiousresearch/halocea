#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_TIE.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::IsAimed @ 0x83296330
// ?IsAimed@aiWEAPON@@UBAHAAUm3dV@@@Z  (virtual)
//
// Is the weapon aimed at world point `trg`? Prefer the selected tie; otherwise return aimed if any
// active registered controller is aimed.
int aiWEAPON::IsAimed(m3dV &trg) const
{
    if (!tieSelected.IsNull())
        return IsAimed(trg, tieSelected);

    const int nElem = ctrlsList.nElem;
    for (int i = 0; i < nElem; ++i) {
        const CTRL_INFO &info = ctrlsList[i];
        propWPN_AI_USABLE *ctrl = info.ctrl.pHandle ? info.ctrl.pHandle->pPtr : nullptr;
        const wpnWEAPON *wpn = ctrl ? ctrl->weapon() : nullptr;
        if (IsCtrlActive(wpn) && IsAimed(trg, ctrl))
            return 1;
    }
    return 0;
}
