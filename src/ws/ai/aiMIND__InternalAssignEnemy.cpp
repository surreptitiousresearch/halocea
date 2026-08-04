// aiMIND::InternalAssignEnemy @ 0x8322B868
// ?InternalAssignEnemy@aiMIND@@IAA_NPAVentENTITY@@_NP6A_NPAVaiBRAIN@@QAVaiWATCHER@@@Z@Z  (protected)
//
// Try to make `ent` this mind's tracked enemy:
//   0. If the descriptor selects the null enemy subsystem, just zero the enemy and fail.
//   1. If we already track `ent`, succeed (no work).
//   2. Otherwise build a watcher for `ent` and run `checker` to accept/reject it (team/eligibility).
//   3. On accept: install a fresh aiENEMY module, seed its visibility, restart the aim module,
//      notify the enemy-changed path, update the "enemy is on the neutral team" status bit, and
//      refresh the planner's cheap-vision cache.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiMIND_arms_boundary.h" // aiENEMY concrete arm
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/ds/ds_new.h"

extern aiPLANNER   *aiPlanner;        // ai08 planner singleton
extern int          IGNORE_STRONG_ASSERT;
extern unsigned char byte_8200155A;   // shared empty-string / assert info byte @0x8200155A

static const char kMindFile[] = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_mind.cpp";

bool aiMIND::InternalAssignEnemy(entENTITY *ent, int isVisible,
                                 bool (*checker)(aiBRAIN *, aiWATCHER *))
{
    aiBRAIN      *brain = static_cast<aiBRAIN *>(pBrain);
    aiBRAIN_DESC *desc  = reinterpret_cast<aiBRAIN_DESC *>(brain->spDesc.pointee);

    if (desc->ssTypeEnemy == aiBRAIN_DESC::aiSUBSYS_NULL)
    {
        ZeroEnemy();
        return false;
    }

    if (enemy.pointee->Is(ent))
        return true; // already tracking this entity as our enemy

    aiWATCHER *wCand = aiWATCHER::Create(ent);
    if (!IGNORE_STRONG_ASSERT && !wCand)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "wCand", kMindFile, 856, byte_8200155A);

    if (!checker(brain, wCand))
    {
        aiWATCHER::Destroy(wCand);
        return false;
    }

    // Capture the previously-tracked entity before we swap the enemy module.
    const entENTITY *entOld = enemy.pointee->GetEntity();

    aiENEMY *enemySys = new (kMindFile, 0x366) aiENEMY(brain, wCand);
    if (!IGNORE_STRONG_ASSERT && !enemySys)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "enemySys", kMindFile, 871, byte_8200155A);

    enemy.Reset(enemySys);
    enemy.pointee->Init(isVisible);
    aim.pointee->ReStart();

    aiWATCHER *peeker = enemy.pointee->GetPeeker();
    NoticeEnemyChanged(peeker, entOld);

    aiSTATUS *st = brain->st.pointee;
    propGAME_INFO_TEAM *enemyTeam = enemy.pointee->GetTeamID();
    dsSTRID neutral;
    gsTEAMS::NeutralTeamID(&neutral);
    if (neutral.id != enemyTeam->team.id)
        st->est.val &= ~0x200000; // enemy is NOT on the neutral team
    else
        st->est.val |= 0x200000;

    if (aiPlanner)
    {
        aiCHEAP_VISION *cheaVis = (&aiPlanner->CheaVis);
        cheaVis->NoticeEnemyChanged(brain, entOld, ent);
    }

    return true;
}
