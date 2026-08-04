#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // m3dRotatePointAroundRay, step_0, _m3dCheckValid, m3dVZero
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiDEBUG::GetSightShape @ 0x8319A0F0  (?GetSightShape@aiDEBUG@@IAAXABUm3dV@@00AAV?$dsVECTOR@Um3dV@@$07@@@Z)
//
// Build the flat sight-cone outline into `shape`: a fan of boundary points swept around the plane
// normal (cross(dirRight, dirForward)) through the eye position `startPos`. Three arcs are emitted —
// the left peripheral arc (radSeeSide), the central arc (radSeeStraight), and the right peripheral
// arc (radSeeSide) — bracketed by `startPos` at both ends.
//
// DEVIATION: the decompiler mis-modeled the m3dRotatePointAroundRay(...) point/out arguments (a
// 5-arg PPC call whose 4th/5th slots it aliased). Disasm shows the swept point is rotated in place
// into the pushed slot; reconstructed accordingly (point == out).
void aiDEBUG::GetSightShape(const m3dV &startPos, const m3dV &dirForward, const m3dV &dirRight,
                            dsVECTOR<m3dV, 8> &shape)
{
    shape.nElem = 0;
    shape.PushBack(startPos);

    _m3dCheckValid(&dirRight);
    _m3dCheckValid(&dirForward);

    // plane normal = cross(dirRight, dirForward)
    m3dV normal;
    normal.x = dirRight.y * dirForward.z - dirRight.z * dirForward.y;
    normal.y = dirForward.x * dirRight.z - dirRight.x * dirForward.z;
    normal.z = dirRight.x * dirForward.y - dirForward.x * dirRight.y;

    const aiSTATUS::PERC &perc = this->pBrain->st.pointee->perc;
    const float radSeeSide       = perc.senses.radSeeSide;
    const float angleSeeSide     = perc.senses.angleSeeSide;
    const float angleSeeStraight = perc.senses.angleSeeStraight;
    const float radSeeStraight   = perc.senses.radSeeStraight;
    const float angleStraightEps = angleSeeStraight + 0.000001f;

    // --- left peripheral arc (radius radSeeSide) ---
    _m3dCheckValid(&dirForward);
    m3dV offSide1 = { dirForward.x * radSeeSide, dirForward.y * radSeeSide, dirForward.z * radSeeSide };
    _m3dCheckValid(&startPos);
    _m3dCheckValid(&offSide1);
    m3dV boundL = { startPos.x + offSide1.x, startPos.y + offSide1.y, startPos.z + offSide1.z };
    for (float a = -angleSeeSide; a < -angleStraightEps; a += step_0)
    {
        m3dV pt = boundL;
        m3dRotatePointAroundRay(&startPos, &normal, a, &pt, &pt);
        shape.PushBack(pt);
    }

    // --- central arc (radius radSeeStraight) ---
    _m3dCheckValid(&dirForward);
    m3dV offCenter = { dirForward.x * radSeeStraight, dirForward.y * radSeeStraight, dirForward.z * radSeeStraight };
    _m3dCheckValid(&startPos);
    _m3dCheckValid(&offCenter);
    m3dV boundC = { startPos.x + offCenter.x, startPos.y + offCenter.y, startPos.z + offCenter.z };
    for (float a = -angleSeeStraight; a < angleStraightEps; a += step_0)
    {
        m3dV pt = boundC;
        m3dRotatePointAroundRay(&startPos, &normal, a, &pt, &pt);
        shape.PushBack(pt);
    }

    // --- right peripheral arc (radius radSeeSide) ---
    _m3dCheckValid(&dirForward);
    m3dV offSide2 = { dirForward.x * radSeeSide, dirForward.y * radSeeSide, dirForward.z * radSeeSide };
    _m3dCheckValid(&startPos);
    _m3dCheckValid(&offSide2);
    m3dV boundR = { startPos.x + offSide2.x, startPos.y + offSide2.y, startPos.z + offSide2.z };
    const float angleSideEps = angleSeeSide + 0.000001f;
    for (float a = angleSeeStraight; a < angleSideEps; a += step_0)
    {
        m3dV pt = boundR;
        m3dRotatePointAroundRay(&startPos, &normal, a, &pt, &pt);
        shape.PushBack(pt);
    }

    shape.PushBack(startPos);
}
