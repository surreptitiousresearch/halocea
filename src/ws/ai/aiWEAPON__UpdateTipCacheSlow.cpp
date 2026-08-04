#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"

// aiWEAPON::UpdateTipCacheSlow @ 0x832959B8
// ?UpdateTipCacheSlow@aiWEAPON@@IAAXABV?$aiW_IDX@VpropWPN_AI_USABLE@@@@@Z
//
// Refresh the cached muzzle tip pos/dir for every strike-vars entry bound to controller `idxCtrl`,
// re-querying the tip individually for each entry (no lazy reuse; used when the controller cannot be
// tip-optimised).
void aiWEAPON::UpdateTipCacheSlow(const aiW_IDX<propWPN_AI_USABLE> &idxCtrl)
{
    const int nEntry = svarsList.nEntry;
    for (int i = 0; i < nEntry; ++i) {
        aiS_VARS &sv = svarsList[i];
        if (sv.tie.idxCtrl.idx == idxCtrl.idx)
            GetPosDirTip(sv.tie, sv.posTip, sv.dirTip);
    }
}
