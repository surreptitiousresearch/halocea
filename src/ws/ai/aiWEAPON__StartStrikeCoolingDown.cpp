#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// aiWEAPON::StartStrikeCoolingDown @ 0x83294A80
// ?StartStrikeCoolingDown@aiWEAPON@@IAAXXZ
//
// Enter the cool-down phase for the selected strike: switch the laser off, park the phase timer, and
// arm the strike-vars cool-down timer from the descriptor's throw-time ramp.
void aiWEAPON::StartStrikeCoolingDown()
{
    SwitchLaser(false);
    strikePhases.phase = AI_SPH_COOLING_DOWN;
    strikePhases.timeToChangePhase = 888888.0f;

    const wpnS_DESC *desc = tieSelected.desc;
    aiS_VARS *vars = tieSelected.vars;
    if (desc && vars)
        vars->timerCoolDown = desc->timeThrow.GetValue();
}
