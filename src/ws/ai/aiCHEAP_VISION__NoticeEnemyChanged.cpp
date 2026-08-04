// aiCHEAP_VISION::NoticeEnemyChanged @ 0x8323FF20
// ?NoticeEnemyChanged@aiCHEAP_VISION@@QAAXPBVaiBRAIN@@PBVentENTITY@@1@Z
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/ds_new.h"

extern aiPLANNER *aiPlanner;      // ?aiPlanner@@3PAVaiPLANNER@@A — ai08 planner singleton
extern unsigned char __ENABLED__; // 0x84325244 — cheap-vision feature/debug enable flag

// Find the cache entry tracking enemy uniqueID `mpID`, or null.
static aiCHEAVIS::VIS_INFO_ENEMY *FindEntry(ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *> &list, unsigned int mpID)
{
    for (ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>::DATA *node = list.head; node; node = node->next)
        if (node->val->mpIDEnemy == mpID)
            return node->val;
    return nullptr;
}

// The tracked enemy of `pBrain` switched from `From` to `To`: leave the old cache entry and join
// (creating it when absent) the new one. Only acts when the brain's team is an enemy of the player.
void aiCHEAP_VISION::NoticeEnemyChanged(const aiBRAIN *pBrain, const entENTITY *From, const entENTITY *To)
{
    if (!__ENABLED__)
        return;

    propGAME_INFO_TEAM *teamID = pBrain->GetTeamID();
    if (!reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams)->IsEnemyToPlayer(*teamID))
        return;

    if (From)
    {
        aiCHEAVIS::VIS_INFO_ENEMY *entry = FindEntry(listVIE, From->uniqueID);
        entry->Leave(pBrain); // original calls Leave on the (possibly null) match — verbatim
    }

    if (To)
    {
        aiCHEAVIS::VIS_INFO_ENEMY *entry = FindEntry(listVIE, To->uniqueID);
        if (!entry)
        {
            entry = new ("D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_cheap_vision.cpp", 339)
                aiCHEAVIS::VIS_INFO_ENEMY(To->uniqueID);
            listVIE.PushBack(entry);
        }
        entry->Join(pBrain);
    }
}
