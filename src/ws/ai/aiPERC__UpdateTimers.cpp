#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_perc_boundary.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiPERC::UpdateTimers() @ 0x8329F768
// ?UpdateTimers@aiPERC@@UAAXM@Z
//
// Virtual override: advance every perception timer by `dt`, accumulate infliction damage at the
// descriptor's rate, decay the injury sub-block, and clear the "on fly-object" / "blocked LOF"
// mind-state bits once their timers expire.
void aiPERC::UpdateTimers(float dt)
{
    m3dUpdateTimeField(&timerClearOnFLOF, dt);
    m3dUpdateTimeField(&timerClearBlockedLOF, dt);
    m3dUpdateTimeField(&timerUpdate, dt);
    m3dUpdateTimeField(&timerNotifyMovingObject, dt);
    if (timerNotifyGrenade > -0.000001f)
        m3dUpdateTimeField(&timerNotifyGrenade, dt);

    const dscDESC *desc = pBrain->spDesc.pointee;
    const float dmgRate = aiBRAIN_DESC_InflictionDmgRate(desc);
    m3dUpdateTimeField(&infliction.timerLocked, dt);
    m3dUpdateTimeField(&infliction.dmgAccumulated, dmgRate * dt);
    injury.Update(dt, aiBRAIN_DESC_InjuryUpdateParam(desc));

    if (timerClearOnFLOF < 0.000001f)
        pBrain->st.pointee->mind.val &= ~0x40;
    if (timerClearBlockedLOF < 0.000001f)
        pBrain->st.pointee->mind.val &= ~0x20;
}
