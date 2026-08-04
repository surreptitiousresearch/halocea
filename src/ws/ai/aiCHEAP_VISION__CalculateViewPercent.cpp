// aiCHEAP_VISION::CalculateViewPercent @ 0x8323FE88
// ?CalculateViewPercent@aiCHEAP_VISION@@QBAMPBVaiBRAIN@@@Z
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ent/entENTITY.h"

// Best current view-percent of `pBrain`'s tracked enemy; 0 when the brain has no enemy entity.
float aiCHEAP_VISION::CalculateViewPercent(const aiBRAIN *pBrain) const
{
    const entENTITY *enemyEnt = pBrain->mind.enemy.pointee->GetEntity();
    if (!enemyEnt)
        return 0.0f;

    aiCHEAVIS::VIS_INFO_ENEMY *entry = nullptr;
    for (ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>::DATA *node = listVIE.head; node; node = node->next)
    {
        if (node->val->mpIDEnemy == enemyEnt->uniqueID)
        {
            entry = node->val;
            break;
        }
    }

    // Original calls the member on the (possibly null) found entry — reproduced verbatim.
    return entry->CalculateViewPercent(pBrain);
}
