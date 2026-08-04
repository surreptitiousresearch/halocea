// aiCHEAP_VISION::PostVisibilityInfo @ 0x83240080
// ?PostVisibilityInfo@aiCHEAP_VISION@@QAAXPBVaiBRAIN@@ABUm3dV@@M@Z
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/m3d/m3dV.h"

extern aiPLANNER *aiPlanner;      // ?aiPlanner@@3PAVaiPLANNER@@A — ai08 planner singleton
extern unsigned char __ENABLED__; // 0x84325244 — cheap-vision feature/debug enable flag

// Forward a fresh sighting of `pSender`'s tracked enemy (view `viewPercent` from `posFrom`) into
// that enemy's cache entry. No-op unless the feature is enabled, the sender has an enemy entity,
// and the sender's team is an enemy of the player.
void aiCHEAP_VISION::PostVisibilityInfo(const aiBRAIN *pSender, const m3dV &posFrom, float viewPercent)
{
    if (!__ENABLED__)
        return;

    const entENTITY *enemyEnt = pSender->mind.enemy.pointee->GetEntity();
    if (!enemyEnt)
        return;

    propGAME_INFO_TEAM *teamID = pSender->GetTeamID();
    if (!reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams)->IsEnemyToPlayer(*teamID))
        return;

    for (ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>::DATA *node = listVIE.head; node; node = node->next)
    {
        if (node->val->mpIDEnemy == enemyEnt->uniqueID)
        {
            node->val->PostVisibilityInfo(pSender->uniqueID, posFrom, viewPercent);
            return;
        }
    }
}
