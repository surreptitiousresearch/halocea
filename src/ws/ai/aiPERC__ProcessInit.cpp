#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/fsm/fsmFLOAT_ARRAY.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero

// aiPERC::ProcessInit @ 0x8329D3D0
// ?ProcessInit@aiPERC@@UAAXXZ
//
// Seed the status block's perception senses (hearing radii + grenade-avoidance radius) and reset
// the radar sub-block from the owned species descriptor at spawn. The descriptor is viewed as a
// 12-byte dscDESC[] cell array; the hearing/avoidance params live in cells 47..48 (matching the
// disassembly's byte offsets).
void aiPERC::ProcessInit()
{
    aiBRAIN *b = pBrain;
    dscDESC *desc = b->spDesc.pointee;
    aiSTATUS *st = b->st.pointee;

    st->perc.senses.radHearShot = *(float *)&desc[47].pBrand;     // cell 47 pBrand
    st->perc.senses.radHearRun  = *(float *)&desc[48].__vftable;  // cell 48 vftable slot
    float walk = *(float *)&desc[48].ref;                          // cell 48 ref slot
    st->radar.blockerUID = 0xFFFFFFFFu;                            // -1: no blocker
    st->perc.senses.radHearWalk = walk;
    st->radar.timeStuck = 0.0f;
    st->radar.timeWaiting = 0.0f;
    st->radar.touch.point = m3dVZero;
    st->radar.touch.isFilled = false;
    st->perc.grAvoidRadius =
        reinterpret_cast<const fsmFLOAT_ARRAY *>(&desc[48].pBrand)->GetValue();
}
