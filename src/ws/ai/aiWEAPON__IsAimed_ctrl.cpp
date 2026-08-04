#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::IsAimed @ 0x83296078
// ?IsAimed@aiWEAPON@@IBA_NABUm3dV@@PBVpropWPN_AI_USABLE@@@Z
//
// Is the weapon aimed at `posTarget` via any strike-vars entry bound to controller `ctrl`? Resolves
// each entry's controller through the ctrls list and, on a match, defers to the per-tie aim test.
bool aiWEAPON::IsAimed(const m3dV &posTarget, const propWPN_AI_USABLE *ctrl) const
{
    if (!ctrl)
        return false;

    const int nEntry = svarsList.nEntry;
    for (int i = 0; i < nEntry; ++i) {
        const aiS_VARS &sv = svarsList[i];
        const int idx = sv.tie.idxCtrl.idx;

        const propWPN_AI_USABLE *entryCtrl = nullptr;
        if (idx >= 0 && idx < ctrlsList.nElem) {
            const CTRL_INFO &info = ctrlsList[idx];
            if (info.ctrl.pHandle)
                entryCtrl = info.ctrl.pHandle->pPtr;
        }

        if (entryCtrl == ctrl && IsAimed(posTarget, sv.tie))
            return true;
    }
    return false;
}
