#include "../../headers/ws/ai/aiDIE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/msg/msgSYSTEM.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/bhp/bhpDEAD_PAR.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern aiPLANNER *aiPlanner;                       // ai08 planner singleton
extern int        gssIsPlayer(const entENTITY *e); // ?gssIsPlayer@@YAHPBVentENTITY@@@Z

// aiDIE::Start(dmgDAMAGE*) @ 0x832AA828
// ?Start@aiDIE@@UAAXPAVdmgDAMAGE@@@Z
//
// Begin the brain's death sequence (only if not already dead): flush delayed calls, notify the
// best-behaviour module, mark the death bit, drop the tracked enemy, schedule the delayed corpse
// message, unregister from the planner/chaser, notify subscribers, upgrade the best module to the
// full form, install the "dead" behaviour, record whether the killer was a player, tell the weapon
// module, and fire the SSL OnComplete for the damage type.
void aiDIE::Start(dmgDAMAGE *dmg)
{
    if (pBrain->IsDead())
        return;

    pBrain->mind.ForceDelayedCalls();
    pBrain->mind.best.pointee->NoticeBeforeDeath();
    pBrain->st.pointee->die.val |= 1u;              // AI_DIEST: dead
    pBrain->mind.ZeroEnemy();

    // AI_GEST bit 0x100000 clear -> schedule the delayed post-death message
    if (((pBrain->st.pointee->gen.val >> 20) & 1) == 0)
        msgSystem.PostTimeMsg(pBrain, 1257, 1.0f, nullptr);

    if (aiPlanner)
    {
        aiPlanner->UnRegisterUnit(pBrain);
        (&aiPlanner->chaser)->NoticeDeath(pBrain);
    }

    msgSystem.NotifySubscribers(1261, nullptr, pBrain);
    pBrain->mind.best.pointee->ConvertToFull(pBrain);

    // AI_DIEST bit 0x2000 clear -> install the "dead" behaviour block
    if (((pBrain->st.pointee->die.val >> 13) & 1) == 0)
    {
        bhpDEAD_PAR deadPar;
        deadPar.dmg = dmg;
        pBrain->mind.best.pointee->SetBehaviorByParams(&deadPar, nullptr);
    }

    ds::WEAK_PTR_HANDLE<entENTITY> *pHandle = dmg->params.entShooter.pHandle;
    const entENTITY *shooter = pHandle ? pHandle->pPtr : nullptr;
    int killedByPlayer = gssIsPlayer(shooter);

    aiSTATUS *status = pBrain->st.pointee;
    if (killedByPlayer)
        status->die.val |= 0x10000;                 // AI_DIEST: killed by player
    else
        status->die.val &= ~0x10000u;

    aiWEAPON_BASE *wpn = pBrain->body.pointee->wpn.pointee;
    if (wpn)
        wpn->OnDieStart();

    dsTSTRING<char> dmgType = dmg->GetType();
    pBrain->mind.CallSSL_OnComplete(dmgType);
}
