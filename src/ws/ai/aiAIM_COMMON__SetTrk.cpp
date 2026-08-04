#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiAIM_COMMON::SetTrk @ 0x8329A638
// ?SetTrk@aiAIM_COMMON@@UAAXPAVentTRACKER@@@Z
//
// Install `pTrk` as the aim tracker, mark the mind's "aim assigned" bit (0x400), then seed all aim
// positions from the tracker's center and publish them immediately.
void aiAIM_COMMON::SetTrk(entTRACKER *pTrk)
{
    trkAim = *pTrk;
    pBrain->st.pointee->mind.val |= 0x400u;

    m3dV center;
    trkAim.GetCenter(&center);
    posToFreeze = center;
    posBasic = center;
    posForProjectile = center;

    FillResults(center, center);
}
