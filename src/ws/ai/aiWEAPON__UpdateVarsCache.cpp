#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON::UpdateVarsCache @ 0x83295DA8
// ?UpdateVarsCache@aiWEAPON@@UAAXXZ  (virtual)
//
// Two passes: (1) refresh each registered controller's cached muzzle tip (fast path when the
// controller allows tip-optimisation, otherwise the slow per-entry path); (2) re-resolve the weapon
// interface and controller weak pointers for every live strike-vars entry and mark it cache-valid.
void aiWEAPON::UpdateVarsCache()
{
    const int nCtrl = ctrlsList.nElem;
    for (int i = 0; i < nCtrl; ++i) {
        aiW_IDX<propWPN_AI_USABLE> idxCtrl;
        idxCtrl.idx = i;
        if (ctrlsList[i].mayOptByTip)
            UpdateTipCacheFast(idxCtrl);
        else
            UpdateTipCacheSlow(idxCtrl);
    }

    const int nEntry = svarsList.nEntry;
    for (int j = 0; j < nEntry; ++j) {
        aiS_VARS &sv = svarsList[j];
        sv.iface.SetPtr(GetIFace(sv.tie));
        sv.ctrl.SetPtr(GetCtrl(sv.tie));
        sv.isCacheValid = true;
    }
}
