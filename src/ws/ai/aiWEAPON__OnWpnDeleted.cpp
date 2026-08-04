#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiTRAJ_CHECKER.h"
#include "../../headers/ws/fsm/fsmWPN_LIST_PARAM_WEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"

// aiWEAPON::OnWpnDeleted @ 0x83294EF0
// ?OnWpnDeleted@aiWEAPON@@IAAXPAUfsmWPN_LIST_PARAM_WEAPON@@@Z
//
// A weapon was removed from the inventory: drop the controller's slot and every strike keyed to it,
// re-enable the surviving strikes, then rebuild the trajectory checker's strike groups.
void aiWEAPON::OnWpnDeleted(fsmWPN_LIST_PARAM_WEAPON *par)
{
    propWPN_AI_USABLE *ctrl = FetchCtrl(par);
    if (ctrl) {
        aiW_IDX<propWPN_AI_USABLE> widx = GetCtrlIDX(ctrl->weapon());
        RemoveCtrlAndStrikes(widx);
        ReeneableStrikes();
        trc.RebuildStrikeGroups();
    }
}
