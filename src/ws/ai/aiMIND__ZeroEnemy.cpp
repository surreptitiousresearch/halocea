#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiBODY_IFACE (body.wpn)
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiPLATT.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiMIND_state_boundary.h" // aiENEMY_ZERO
#include "../../headers/ws/ds/ds_new.h"

extern aiPLANNER *aiPlanner;                       // ai08 planner singleton
extern int        gssIsPlayer(const entENTITY *e); // ?gssIsPlayer@@YAHPBVentENTITY@@@Z

// aiMIND::ZeroEnemy @ 0x8322B348
// ?ZeroEnemy@aiMIND@@QAAXXZ
//
// Drop the brain's tracked enemy: install the null "zero" enemy arm, restart aiming, and tell the
// planner's cheap-vision cache / player-attacker registry / weapon module that the enemy is gone.
// Finally clear the status enemy sub-block and the "enemy visible" estimate flag.
void aiMIND::ZeroEnemy()
{
    if (enemy.pointee && enemy.pointee->IsZero())
        return;

    aiBRAIN         *brain  = static_cast<aiBRAIN *>(pBrain);
    const entENTITY *entOld = enemy.pointee ? enemy.pointee->GetEntity() : nullptr;

    enemy.Reset<aiENEMY_ZERO>(new aiENEMY_ZERO(brain));
    enemy.pointee->Init(0);
    aim.pointee->ReStart();

    if (aiPlanner)
    {
        (&aiPlanner->CheaVis)
            ->NoticeEnemyChanged(brain, entOld, nullptr);
        if (gssIsPlayer(entOld))
            (&aiPlanner->platt)->RemoveAttacker(brain);
    }

    aiWEAPON_BASE *wpn = brain->body.pointee->wpn.pointee;
    wpn->NoticeEnemyChanged();

    if (aiPlanner)
        (&aiPlanner->CheaVis)
            ->NoticeEnemyChanged(brain, entOld, nullptr);

    aiSTATUS *st = brain->st.pointee;
    st->enemy.Reset();
    st->est.val &= ~0x200000u; // clear "enemy visible/known" estimate bit
}
