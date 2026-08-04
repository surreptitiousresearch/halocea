#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h" // m3dVZero, _m3dCheckValid, M3D_EPSILON_2_623
#include "../../headers/ws/m3d/m3dV.h"
#include <math.h>

// aiWEAPON::UpdateTipCacheFast @ 0x83295868
// ?UpdateTipCacheFast@aiWEAPON@@IAAXABV?$aiW_IDX@VpropWPN_AI_USABLE@@@@@Z
//
// Refresh the cached muzzle tip pos/dir for every strike-vars entry bound to controller `idxCtrl`.
// "Fast": compute the tip once (lazily, when the running scratch value is still ~zero) and reuse it
// for all matching entries, rather than re-querying per entry.
void aiWEAPON::UpdateTipCacheFast(const aiW_IDX<propWPN_AI_USABLE> &idxCtrl)
{
    const int nEntry = svarsList.nEntry;
    m3dV posTip = m3dVZero;
    m3dV dirTip = m3dVZero;

    for (int i = 0; i < nEntry; ++i) {
        aiS_VARS &sv = svarsList[i];
        if (sv.tie.idxCtrl.idx != idxCtrl.idx)
            continue;

        _m3dCheckValid(&posTip);
        const float lenSq = posTip.x * posTip.x + (posTip.z * posTip.z + posTip.y * posTip.y);
        if (fabsf(lenSq) < M3D_EPSILON_2_623)
            GetPosDirTip(sv.tie, posTip, dirTip);

        sv.posTip = posTip;
        sv.dirTip = dirTip;
    }
}
