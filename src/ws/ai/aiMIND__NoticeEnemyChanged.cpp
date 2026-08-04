#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiBODY_IFACE (body.wpn)
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiPLATT.h"
#include "../../headers/ws/ai/aiPLANNER.h"

extern aiPLANNER *aiPlanner;                       // ai08 planner singleton
extern int        gssIsPlayer(const entENTITY *e); // ?gssIsPlayer@@YAHPBVentENTITY@@@Z

// aiMIND::NoticeEnemyChanged @ 0x83228370
// ?NoticeEnemyChanged@aiMIND@@QAAXPBVaiWATCHER@@PBVentENTITY@@@Z
//
// The tracked enemy switched from `entOld` to whatever `wNew` watches (null when there is no new
// enemy). Update the planner's cheap-vision cache and player-attacker registry, then let the weapon
// module react.
void aiMIND::NoticeEnemyChanged(const aiWATCHER *wNew, const entENTITY *entOld)
{
    const entENTITY *entNew = wNew ? wNew->GetEntity() : nullptr;

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    if (aiPlanner)
    {
        aiCHEAP_VISION *cheaVis = (&aiPlanner->CheaVis);
        cheaVis->NoticeEnemyChanged(brain, entOld, entNew);

        aiPLATT *platt = (&aiPlanner->platt);
        if (gssIsPlayer(entOld))
            platt->RemoveAttacker(brain);
        if (wNew && wNew->IsPlayer())
            platt->AddAttacker(brain);
    }

    aiWEAPON_BASE *wpn = brain->body.pointee->wpn.pointee;
    wpn->NoticeEnemyChanged();
}
