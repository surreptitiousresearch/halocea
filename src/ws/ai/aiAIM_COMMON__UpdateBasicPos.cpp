#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero

// aiAIM_COMMON::UpdateBasicPos @ 0x83299CB8
// ?UpdateBasicPos@aiAIM_COMMON@@IAAXXZ  (protected)
//
// Refresh the basic aim position. Latch this frame's freeze state (mind bit 0x200), clear the
// "recompute" request bit (0x100), then recompute posBasic via CalcBasicPos; posToFreeze mirrors it.
void aiAIM_COMMON::UpdateBasicPos()
{
    aiSTATUS *status = pBrain->st.pointee;
    isFrozen = (status->mind.val & 0x200) != 0;

    m3dV basic = m3dVZero;
    status->mind.val &= ~0x100u;
    CalcBasicPos(basic);

    posBasic = basic;
    posToFreeze = basic;
}
