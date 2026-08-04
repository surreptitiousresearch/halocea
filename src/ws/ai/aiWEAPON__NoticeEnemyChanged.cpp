#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_abilities_boundary.h"

// aiWEAPON::NoticeEnemyChanged @ 0x83294328
// ?NoticeEnemyChanged@aiWEAPON@@UAAXXZ
//
// The tracked enemy just changed: if the species may not change enemy during a strike and we are
// mid-aim on a live strike, cancel it (its aim is now stale).
void aiWEAPON::NoticeEnemyChanged()
{
    if (!aiBRAIN_DESC_AbilChangeEnemyDuringStrike(pBrain->spDesc.pointee)
        && IsShootingNow()
        && strikePhases.phase == AI_SPH_AIMING) {
        CancelCurStrike(false);
    }
}
