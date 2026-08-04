#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_abilities_boundary.h"

// aiWEAPON::MayChangeEnemy @ 0x832942D0
// ?MayChangeEnemy@aiWEAPON@@UBA_NXZ
//
// May the brain switch to a new enemy now? Always allowed for species whose descriptor permits
// changing enemy during a strike; otherwise only when not currently firing.
bool aiWEAPON::MayChangeEnemy() const
{
    if (aiBRAIN_DESC_AbilChangeEnemyDuringStrike(pBrain->spDesc.pointee))
        return true;
    return !IsShootingNow();
}
