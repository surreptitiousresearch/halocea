#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/fsm/fsmWPN_LIST_PARAM_WEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON::OnWpnActivated @ 0x83294F58
// ?OnWpnActivated@aiWEAPON@@IAAXPAUfsmWPN_LIST_PARAM_WEAPON@@@Z
//
// A weapon became active: register its controller and strikes if not already present, mark the
// controller active, re-enable strikes, and refresh the module's optimizations.
void aiWEAPON::OnWpnActivated(fsmWPN_LIST_PARAM_WEAPON *par)
{
    propWPN_AI_USABLE *ctrl = FetchCtrl(par);
    if (ctrl) {
        if (IsAbsent(ctrl))
            AddCtrlAndStrikes(ctrl);
        ActivateCtrl(ctrl, true);
        ReeneableStrikes();
        UpdateOptimizations();
    }
}
