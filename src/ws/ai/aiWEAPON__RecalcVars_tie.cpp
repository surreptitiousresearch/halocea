#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"

// aiWEAPON::RecalcVars @ 0x83293D58
// ?RecalcVars@aiWEAPON@@IAAXAAVaiS_TIE@@@Z
//
// Thin adapter: build an aiSTRIKE from a tie (vars/desc plus the resolved weapon interface) and
// forward to the aiSTRIKE-keyed recompute. No-op unless vars, desc, and the interface all resolve.
void aiWEAPON::RecalcVars(aiS_TIE &tie)
{
    aiS_VARS *vars = tie.vars;
    const wpnS_DESC *desc = tie.desc;
    wpnWEAPON *iface = GetIFace(tie.idxCtrl);
    if (vars && desc && iface) {
        aiSTRIKE strike;
        strike.wpn = iface;
        strike.vars = vars;
        strike.desc = desc;
        strike.weight = 0.0f;
        RecalcVars(strike);
    }
}
