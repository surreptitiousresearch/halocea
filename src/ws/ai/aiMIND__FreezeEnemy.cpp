#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ai/aiMIND_state_boundary.h" // aiENEMY_FROZEN
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::FreezeEnemy @ 0x8322B4E0
// ?FreezeEnemy@aiMIND@@QAAXXZ
//
// Replace the tracked enemy with the "frozen" arm (the unit keeps a stale enemy fix while realizing
// the target's death), restart aiming, and notify the change through the original enemy's peeker.
void aiMIND::FreezeEnemy()
{
    if (enemy.pointee && enemy.pointee->IsFrozen())
        return;

    aiBRAIN         *brain  = static_cast<aiBRAIN *>(pBrain);
    const entENTITY *entOld = enemy.pointee ? enemy.pointee->GetEntity() : nullptr;

    enemy.Reset<aiENEMY_FROZEN>(new aiENEMY_FROZEN(brain));
    aim.pointee->ReStart();

    aiWATCHER *peeker = enemy.pointee->GetPeeker();
    NoticeEnemyChanged(peeker, entOld);
}
