#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// aiWEAPON::UpdateLaserState @ 0x83294740
// ?UpdateLaserState@aiWEAPON@@IAAX_N@Z
//
// Drive the laser on/off request `enable` through the current enemy state: force it off (regardless
// of the request) when there is no live enemy — but only for strikes whose descriptor uses a laser.
void aiWEAPON::UpdateLaserState(bool enable)
{
    const wpnS_DESC *desc = tieSelected.desc;
    if (desc && desc->isUseLaser) {
        aiENEMY_BASE *enemy = pBrain->mind.enemy.pointee;
        bool on = enable;
        if (enemy->Is(nullptr) || enemy->IsDead())
            on = false;
        SwitchLaser(on);
    }
}
