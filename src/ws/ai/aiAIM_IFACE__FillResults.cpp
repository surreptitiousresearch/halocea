#include "../../headers/ws/ai/aiAIM_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiCMD2BODY.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiAIM_IFACE::FillResults @ 0x83299C58
// ?FillResults@aiAIM_IFACE@@UAAXABUm3dV@@0@Z
//
// Publish the resolved aim/weapon-target positions into the brain's per-frame command block. The
// look-at is only forced when the body's "external look-at set" state bit (0x400000) is clear.
void aiAIM_IFACE::FillResults(const m3dV &newPosAim, const m3dV &newPosWpnTgt)
{
    pBrain->cmd.posWpnTgt = newPosWpnTgt;
    pBrain->cmd.posAim = newPosAim;
    if (((pBrain->st.pointee->body.val >> 22) & 1) == 0)
        pBrain->cmd.SetPosLookAt(newPosAim, false);
}
