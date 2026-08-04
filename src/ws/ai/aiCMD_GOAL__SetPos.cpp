#include "../../headers/ws/ai/aiBRAIN.h"     // pulls aiCMD_GOAL + aiBRAIN (pBrain->st)
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // _m3dNormalize

extern const m3dV  m3dVZero;          // ?m3dVZero@@3Um3dV@@A — global zero vector (boundary)
extern const float M3D_EPSILON_2_538; // squared-distance epsilon constant (boundary)
extern float m3dDist_2(const m3dV *a, const m3dV *b); // squared distance (boundary)

// aiCMD_GOAL::SetPos @ 0x831783F8
// ?SetPos@aiCMD_GOAL@@QAAXPAVaiBRAIN@@ABUm3dV@@M@Z
//
// Set the movement goal position. `dist` scales the reach radius by distReachMultiplier (a negative
// `dist` means "half the multiplier"). When the goal actually moved, recompute the approach
// direction from `pBrain`'s body to the new goal and flag a big turn on the brain's status.
// (Decompiler mistyped the pBrain parameter as m3dV*; the mangled name confirms aiBRAIN*.)
void aiCMD_GOAL::SetPos(aiBRAIN *pBrain, const m3dV &pos, float dist)
{
    if (dist >= 0.0f)
        distReach = distReachMultiplier * dist;
    else
        distReach = distReachMultiplier * 0.5f;

    if (m3dDist_2(&posGoal, &pos) < M3D_EPSILON_2_538)
        return; // goal unchanged

    posGoal = pos;
    posChkReach = pos;

    // Body world position via the brain's virtual accessor (vtable byte offset 0x1C0).
    m3dV bodyPos;
    typedef void (*GetBodyPosFn)(m3dV *out, aiBRAIN *self);
    void **vtbl = *reinterpret_cast<void ***>(pBrain);
    reinterpret_cast<GetBodyPosFn>(vtbl[0x1C0 / sizeof(void *)])(&bodyPos, pBrain);

    m3dV dir;
    dir.x = pos.x - bodyPos.x;
    dir.y = pos.y - bodyPos.y;
    dir.z = pos.z - bodyPos.z;
    if (!_m3dNormalize(&dir))
        dir = m3dVZero;

    aiSTATUS *pStatus = pBrain->st.pointee;
    if (DetectBigTurn(dir))
        pStatus->body.val |= 0x20;      // big-turn flag
    else
        pStatus->body.val &= ~0x20;

    dirGoal = dir;
}
