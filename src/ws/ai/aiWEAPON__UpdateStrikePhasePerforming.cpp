#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/ds/dsSTRID.h"

// ?_apLog@@YAXPBDZZ — Saber logging sink (variadic). boundary.
void _apLog(const char *format, ...);

// aiWEAPON::UpdateStrikePhasePerforming @ 0x832950B0
// ?UpdateStrikePhasePerforming@aiWEAPON@@IAAXXZ
//
// While the selected strike is performing, watch the controller's strike status: if it went inactive
// or paused, begin cool-down; if it reports "performed", cancel the strike either because an
// animation demands it, or because a non-hang strike has overstayed (kill-as-hang) once the hack
// timer expires.
void aiWEAPON::UpdateStrikePhasePerforming()
{
    propWPN_AI_USABLE *ctrl = GetCtrl(tieSelected);
    if (!ctrl)
        return;

    wpnSTRIKE_STATUS status = ctrl->GetStrikeStatus(tieSelected.desc);
    if (status == STRIKE_INACTIVE || status == STRIKE_IN_PAUSE) {
        StartStrikeCoolingDown();
    } else if (status == STRIKE_PERFORMED) {
        if (IsNeedToCancelStrikeByAnim()) {
            CancelCurStrike(false);
        } else if (!tieSelected.desc->isAllowHang && timerHackKillHang < 0.000001f) {
            dsSTRID strikeName = tieSelected.name;
            _apLog("~AI~'%s': strike '%s' killed as hang", pBrain->names.nameInst,
                   strikeName.CStr());
            CancelCurStrike(false);
        }
    }
}
