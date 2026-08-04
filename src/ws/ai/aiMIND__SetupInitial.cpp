// aiMIND::SetupInitial @ 0x8322C258
// ?SetupInitial@aiMIND@@QAAXXZ
//
// Per-level initial mind setup, under the "init all" profiling counter. Bails (clearing the gen
// 0x20 "needs setup" bit) when the unit is in a revive-wait state or is dead-without-the-init bit.
// When CanSetupInitial() is not yet true, arms the gen 0x20 bit and returns. Otherwise, under the
// nested "init/delta" counter: tick the self-watcher, register the unit with the nav penalty manager
// and the statistics block, set the "enemy-to-player" est bit from the team relation, localize the
// unit to a nav system, pick the initial behaviour, run enemy selection, and tick the aim module.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

extern aiPLANNER *aiPlanner; // ai08 planner singleton

void aiMIND::SetupInitial()
{
    static apCOUNTER_TIME cAll("process_frame/ai/brain/gs/init all", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeAll(&cAll);

    aiBRAIN  *brain = static_cast<aiBRAIN *>(pBrain);
    aiSTATUS *st    = brain->st.pointee;

    bool isReviveWait  = ((st->die.val >> 20) & 1) != 0;
    bool deadWithoutBit = brain->IsDead() && (((brain->st.pointee->die.val >> 1) & 1) == 0);
    if (isReviveWait || deadWithoutBit)
    {
        brain->st.pointee->gen.val &= ~0x20; // no longer requesting setup
        return;
    }

    if (!CanSetupInitial())
    {
        brain->st.pointee->gen.val |= 0x20; // still waiting to be set up
        return;
    }

    brain->st.pointee->gen.val &= ~0x20;

    {
        static apCOUNTER_TIME cDelta("process_frame/ai/brain/gs/init/delta", nullptr);
        apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeDelta(&cDelta);

        aiWATCHER_BRAIN *peekerSelf = brain->st.pointee->peekerSelf;
        peekerSelf->Update(0.0f);

        navPENMAN *pen = aiPlanner->GetPenaltyManager();
        pen->AddUnit(brain);

        (&aiPlanner->stat)->NoticeUnitCreated(brain);

        propGAME_INFO_TEAM *team = brain->GetTeamID();
        bool isEnemyToPlayer =
            reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams)->IsEnemyToPlayer(*team);

        aiSTATUS *st2 = brain->st.pointee;
        if (isEnemyToPlayer)
            st2->est.val |= 0x100000;
        else
            st2->est.val &= ~0x100000;
    }

    aiPlanner->LocalizeToNavSystem(brain);
    best.pointee->SetupInitialBehavior();
    ProcessEnemySelection();
    aim.pointee->ProcessFrame();
}
