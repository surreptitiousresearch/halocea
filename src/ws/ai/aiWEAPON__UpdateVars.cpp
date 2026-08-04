#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/ai/aiS_VARS.h"

// aiWEAPON::UpdateVars @ 0x83294060
// ?UpdateVars@aiWEAPON@@IAAXXZ
//
// Per-frame: recompute the cached strike variables for every strike that is currently enabled by
// script, filter and controller and resolves to a live (vars, descriptor, weapon-interface) triple.
void aiWEAPON::UpdateVars()
{
    for (int i = 0; i < svarsList.nEntry; ++i) {
        aiS_VARS &v = svarsList[i];
        if (v.isEnabledByScript && v.isEnabledByFilter && v.isEnabledByCtrl) {
            aiS_VARS *vars = v.tie.vars;
            const wpnS_DESC *desc = v.tie.desc;
            wpnWEAPON *iface = GetIFace(v.tie.idxCtrl);
            if (vars && desc && iface) {
                aiSTRIKE strike;
                strike.wpn = iface;
                strike.weight = 0.0f;
                strike.vars = vars;
                strike.desc = desc;
                RecalcVars(strike);
            }
        }
    }
}
