#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/fsm/fsmWPN_LIST_PARAM_WEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiWEAPON::OnWpnDeactivated @ 0x83295240
// ?OnWpnDeactivated@aiWEAPON@@IAAXPAUfsmWPN_LIST_PARAM_WEAPON@@@Z
//
// A weapon became inactive: ensure its controller/strikes are registered, then mark the controller
// inactive, re-enable the remaining strikes, and refresh optimizations.
void aiWEAPON::OnWpnDeactivated(fsmWPN_LIST_PARAM_WEAPON *par)
{
    propWPN_AI_USABLE *ctrl = FetchCtrl(par);
    if (ctrl) {
        if (IsAbsent(ctrl))
            AddCtrlAndStrikes(ctrl);
        ActivateCtrl(ctrl, false);
        ReeneableStrikes();
        UpdateOptimizations();
    }
}
