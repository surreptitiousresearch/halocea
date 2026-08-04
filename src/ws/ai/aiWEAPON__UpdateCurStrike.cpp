#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiWEAPON::UpdateCurStrike @ 0x83295308
// ?UpdateCurStrike@aiWEAPON@@IAAXXZ
//
// Per-frame update of the currently selected strike: age the phase-change timer, recompute the
// strike vars while the interface/vars/desc are all live, request an aim freeze when the descriptor
// asks for one, then advance the strike phase state machine.
void aiWEAPON::UpdateCurStrike()
{
    if (tieSelected.IsNull())
        return;

    float dt = pBrain->GetElapsedTime();
    float diff = 888888.0f - strikePhases.timeToChangePhase; // 888888 == "no timed transition"
    if ((diff < 0.0f ? -diff : diff) >= 0.000001f)
        m3dUpdateTimeField(&strikePhases.timeToChangePhase, dt);

    aiS_VARS *vars = tieSelected.vars;
    const wpnS_DESC *desc = tieSelected.desc;
    wpnWEAPON *iface = GetIFace(tieSelected.idxCtrl);
    if (vars && desc && iface) {
        aiSTRIKE strike;
        strike.wpn = iface;
        strike.vars = vars;
        strike.desc = desc;
        strike.weight = 0.0f;
        RecalcVars(strike);
    }

    const wpnS_DESC *selDesc = tieSelected.desc;
    if (selDesc && selDesc->isNeedToFreezeAim) {
        aiSTATUS *st = pBrain->st.pointee;
        if (st->aim.timerToFreezeAim < 0.000001f)
            st->mind.val |= 0x200; // AI_MIST freeze-aim request bit
    }

    aiS_PHASES phase = strikePhases.phase;
    if ((unsigned int)phase <= (unsigned int)AI_SPH_NONE) {
        if (phase == AI_SPH_PERFORMING)
            UpdateStrikePhasePerforming();
        else if (phase == AI_SPH_COOLING_DOWN)
            UpdateStrikePhaseCoolingDown();
        else if (phase)                 // AI_SPH_NONE
            ForgetSelectedStrike();
        else                            // AI_SPH_AIMING
            UpdateStrikePhaseAiming();
    }
}
