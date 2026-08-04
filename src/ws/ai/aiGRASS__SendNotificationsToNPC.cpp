#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ent/entENTITY.h"

// ai08 planner singleton (?aiPlanner@@3PAVaiPLANNER@@A). boundary data.
extern aiPLANNER *aiPlanner;
// 0x8265BB00 (?m3dDist@@YAMPIBUm3dV@@0@Z) — Euclidean distance between two points. boundary.
extern float m3dDist(const m3dV *a, const m3dV *b);

// aiGRASS::SendNotificationsToNPC @ 0x83183518
// ?SendNotificationsToNPC@aiGRASS@@IAAXXZ  (protected)
//
// For every tracked grenade, refresh its last-known centre and then alert every live enemy
// unit-watcher standing within its grenade-avoid radius.
void aiGRASS::SendNotificationsToNPC()
{
    for (ds::LIST<GRENADE_INFO>::DATA *node = this->grenades.head; node; node = node->next)
    {
        entENTITY *grenade = const_cast<entENTITY *>(node->val.grenade);
        grenade->GetCenter(&node->val.posLastKnown);

        int count = aiPlanner->wah.arrUnits.nElem;
        for (int idx = 0; idx < count; ++idx)
        {
            aiWATCHER_BRAIN *unit = aiPlanner->wah.PeekActive(idx);
            if (!unit || unit->IsDead())
                continue;

            dsSTRID unitTeam = unit->GetTeamID()->team;
            aiTEAMS_SYS *teams = reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams);
            if (!teams->IsEnemy(node->val.shooterTeamID, unitTeam))
                continue;

            m3dV unitPos;
            unit->GetBodyPos(&unitPos);
            float avoidRadius = unit->GetRadiusAvoidGrenade();
            if (m3dDist(&node->val.posLastKnown, &unitPos) <= avoidRadius)
                unit->NoticeGrenade(grenade);
        }
    }
}
