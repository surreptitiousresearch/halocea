#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_perc_boundary.h"
#include "../../headers/ws/ai/aiPERC_flyobj_boundary.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/mdl/mdlCDT_SETTINGS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiPERC::ChoosePool() @ 0x8329E678
// ?ChoosePool@aiPERC@@IBAPBV?$dsVECTOR@VdsSTRID@@$07@@M_N@Z
//
// Pick the descriptor's fly-object (grenade / physics-object) reaction FSM-event pool for a
// stimulus seen at distance `dist` on the given side. When the stimulus is inside the react band,
// a body-right shoot-cylinder tests whether the dodge line-of-fire is clear; if so, the "far"
// pool is used, otherwise (blocked, still in band) no pool is returned. Below the band the "near"
// pool is used, above it nothing.
//
// DEVIATION: the mangled name proves two params (float, bool) — the disassembly (call site sets
// only r3/f1/r5) confirms it. The decompiler invented a spurious middle `isOnRightSide`; the
// register actually carrying the side flag is r5 (its `a4`), which is `onRightSide` here.
const dsVECTOR<dsSTRID, 8> *aiPERC::ChoosePool(float dist, bool onRightSide) const
{
    const dscDESC *desc = pBrain->spDesc.pointee;
    const float distMin = aiBRAIN_DESC_FlyObjReactDistMin(desc);
    const float distMax = aiBRAIN_DESC_FlyObjReactDistMax(desc);

    if (distMin <= dist && dist <= distMax)
    {
        const float sign = onRightSide ? 1.0f : -1.0f;
        const m3dV bodyPos = pBrain->GetBodyPosNav();
        const aiSTATUS *st = pBrain->st.pointee;
        const m3dV probeEnd = bodyPos + (sign * jumpDist_0) * st->cs.dirRightBody;

        if (!aiPERC_FlyObjProbeBlocked(pBrain, bodyPos, probeEnd))
        {
            return onRightSide ? aiBRAIN_DESC_FlyObjReactFarRight(desc)
                               : aiBRAIN_DESC_FlyObjReactFarLeft(desc);
        }
    }

    if (dist >= distMin)
        return nullptr;

    return onRightSide ? aiBRAIN_DESC_FlyObjReactNearRight(desc)
                       : aiBRAIN_DESC_FlyObjReactNearLeft(desc);
}
