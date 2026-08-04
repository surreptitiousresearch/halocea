#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/ai/aiS_VARS.h"

// aiWEAPON::DbgRecalcVars @ 0x83293DB8
// ?DbgRecalcVars@aiWEAPON@@UAAXXZ
//
// Debug: force a full recompute of every strike-vars block that currently resolves to a live
// (vars, descriptor, weapon-interface) triple — used to refresh the strike debug overlay.
void aiWEAPON::DbgRecalcVars()
{
    for (int i = 0; i < svarsList.nEntry; ++i) {
        aiS_VARS &v = svarsList[i];
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
