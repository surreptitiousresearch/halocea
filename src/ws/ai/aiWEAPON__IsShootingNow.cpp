#include "../../headers/ws/ai/aiWEAPON.h"

// aiWEAPON::IsShootingNow @ 0x83293348
// ?IsShootingNow@aiWEAPON@@UBA_NXZ
//
// True while a strike is selected and its phase is still in the active range (aiming/performing,
// i.e. strictly before COOLING_DOWN).
bool aiWEAPON::IsShootingNow() const
{
    if (tieSelected.IsNull())
        return false;
    return (unsigned int)strikePhases.phase < (unsigned int)AI_SPH_COOLING_DOWN;
}
