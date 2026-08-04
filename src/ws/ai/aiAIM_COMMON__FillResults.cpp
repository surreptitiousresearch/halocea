#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiCMD2BODY.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiAIM_COMMON::FillResults @ 0x83299FD0
// ?FillResults@aiAIM_COMMON@@UAAXABUm3dV@@0@Z
//
// Publish the basic/projectile aim positions into the status block and the per-brain debug mirror,
// then push the resolved aim/weapon-target positions into the command block (forcing the look-at
// unless the body's external-look-at state bit 0x400000 is set).
void aiAIM_COMMON::FillResults(const m3dV &newPosAim, const m3dV &newPosWpnTgt)
{
    aiSTATUS *status = pBrain->st.pointee;
    status->aim.posBasicTarget = posBasic;
    status->aim.pointForProjectile = posForProjectile;

    pBrain->dbg.posBasicAiming = posBasic;
    pBrain->dbg.pointForProjectile = posForProjectile;

    pBrain->cmd.posWpnTgt = newPosWpnTgt;
    pBrain->cmd.posAim = newPosAim;
    if (((pBrain->st.pointee->body.val >> 22) & 1) == 0)
        pBrain->cmd.SetPosLookAt(newPosAim, false);
}
