#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiWEAPON::UpdateStrikePhaseAiming @ 0x83294980
// ?UpdateStrikePhaseAiming@aiWEAPON@@IAAXXZ
//
// Aiming-phase step of the selected strike: bail (cancel) if the controller is gone or an animation
// forces an aim cancel; otherwise hold the laser on and, if advanced-sniper aiming is active, gate
// the phase timer on its finished flag. When the aim timer elapses, transition to performing (or
// cancel if the strike could not start).
void aiWEAPON::UpdateStrikePhaseAiming()
{
    if (!GetCtrl(tieSelected) || IsNeedToCancelAimByAnim()) {
        CancelCurStrike(false);
        return;
    }

    UpdateLaserState(true);

    aiSTATUS *st = pBrain->st.pointee;
    if (st->aim.advSniper.params.isEnabled)
        strikePhases.timeToChangePhase = st->aim.advSniper.isFinished ? 0.0f : 888888.0f;

    if (strikePhases.timeToChangePhase < 0.000001f) {
        if (StartStrikePerforming()) {
            strikePhases.timeToChangePhase = 888888.0f;
            strikePhases.phase = AI_SPH_PERFORMING;
            return;
        }
        CancelCurStrike(false);
    }
}
