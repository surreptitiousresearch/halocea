#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// aiWEAPON::UpdateStrikePhaseCoolingDown @ 0x83292C08
// ?UpdateStrikePhaseCoolingDown@aiWEAPON@@IAAXXZ
//
// While the selected strike is cooling down, decide when to release it: if the controller/vars are
// gone, cancel outright; once the cool-down timer has elapsed and the strike is no longer paused,
// either forget it (normal weapons) or cancel-with-skip (body-check-skipping weapons).
void aiWEAPON::UpdateStrikePhaseCoolingDown()
{
    propWPN_AI_USABLE *ctrl = GetCtrl(tieSelected);
    aiS_VARS *vars = tieSelected.vars;
    if (!ctrl || !vars) {
        CancelCurStrike(false);
        return;
    }
    if (vars->timerCoolDown < 0.000001f
        && ctrl->GetStrikeStatus(tieSelected.desc) != STRIKE_IN_PAUSE) {
        if (!tieSelected.desc->isSkipBodyChecks) {
            ForgetSelectedStrike();
            return;
        }
        CancelCurStrike(true);
    }
}
