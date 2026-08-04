// aiDEBUG::CreateVisionCone @ 0x8319A5A8
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h"  // m3dRotatePointAroundRay, _m3dCheckValid,
                                                            // m3dV operator +/-/*, dlFree
#include "../../headers/ws/ai/aiDEBUG_render2_boundaries.h" // step_1
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// Build the multi-ring vision-cone polylines from the eye position and the forward/right directions.
//
// For each roll slice (0..180 deg about dirForward) an outline polyline is emitted that traces the
// field-of-view boundary: the peripheral (side) radius from -angleSeeSide down to -angleSeeStraight,
// the forward radius across [-angleSeeStraight, +angleSeeStraight], and the peripheral radius back
// out to +angleSeeSide. Three closed "rib" rings (peripheral-outer, peripheral-inner, forward) are
// accumulated across slices and appended last.
//
// NOTE: m3dRotatePointAroundRay is the 5-arg (rayOrigin, rayDir, angle, pointIn, out) form; the
// decompiler mis-tracks the point/out registers for this helper — the geometry below reflects the
// documented signature (see aiDEBUG_render_boundaries.h).
void aiDEBUG::CreateVisionCone(const m3dV *startPos, const m3dV *dirForward, const m3dV *dirRight,
                               dsVECTOR<dsVECTOR<m3dV, 8>, 8> *visCone)
{
    visCone->Clear();

    aiSTATUS *st = this->pBrain->st.pointee;
    const float angleStraight = st->perc.senses.angleSeeStraight; // pointee[382]
    const float angleSide     = st->perc.senses.angleSeeSide;     // pointee[383]
    const float radStraight   = st->perc.senses.radSeeStraight;   // pointee[384]
    const float radSide       = st->perc.senses.radSeeSide;       // pointee[385]

    // Rib rings accumulated one point per slice; the "High" ring is folded into the "Low" ring and
    // closed at the end. Peripheral rings use radSide, the forward ring uses radStraight.
    dsVECTOR<m3dV, 8> ringSideLow, ringSideHigh;         // v80 / v90  (+/- angleSide,   radSide)
    dsVECTOR<m3dV, 8> ringPeriphLow, ringPeriphHigh;     // v82 / v91  (+/- angleStraight,radSide)
    dsVECTOR<m3dV, 8> ringFwdLow, ringFwdHigh;           // v81 / v92  (+/- angleStraight,radStraight)

    _m3dCheckValid(dirRight);
    _m3dCheckValid(dirForward);

    // Cone "up" axis = cross(dirRight, dirForward); the per-slice rotation axis is this vector rolled
    // about dirForward.
    m3dV coneUp;
    coneUp.x = dirRight->y * dirForward->z - dirRight->z * dirForward->y;
    coneUp.y = dirForward->x * dirRight->z - dirRight->x * dirForward->z;
    coneUp.z = dirRight->x * dirForward->y - dirForward->x * dirRight->y;

    m3dV rotated; // reused rotate output slot (v72)

    for (float rollDeg = 0.0f; rollDeg < 180.0f; rollDeg += 10.0f)
    {
        _m3dCheckValid(startPos);
        _m3dCheckValid(&coneUp);
        m3dV upPoint = *startPos + coneUp;
        m3dV rotatedUp;
        m3dRotatePointAroundRay(startPos, dirForward, rollDeg, &upPoint, &rotatedUp);
        _m3dCheckValid(&rotatedUp);
        _m3dCheckValid(startPos);
        m3dV sliceAxis = rotatedUp - *startPos;

        dsVECTOR<m3dV, 8> sliceOutline; // v86
        sliceOutline.PushBack(*startPos);

        // Peripheral spoke point (radSide along forward), swept about the slice axis.
        m3dV sidePoint = *startPos + (*dirForward * radSide); // v87

        // -angleSide edge → ringSideLow.
        m3dRotatePointAroundRay(startPos, &sliceAxis, -angleSide, &sidePoint, &rotated);
        ringSideLow.PushBack(rotated);

        // Peripheral arc from -angleSide up to -angleStraight.
        int nSide = (int)((angleSide - angleStraight) / step_1);
        for (int i = 0; i <= nSide; ++i)
        {
            float a = i * step_1 - angleSide;
            if (a > -angleStraight)
                a = -angleStraight;
            m3dRotatePointAroundRay(startPos, &sliceAxis, a, &sidePoint, &rotated);
            sliceOutline.PushBack(rotated);
        }
        m3dRotatePointAroundRay(startPos, &sliceAxis, -angleStraight, &sidePoint, &rotated);
        ringPeriphLow.PushBack(rotated);

        // Forward spoke point (radStraight along forward).
        m3dV fwdPoint = *startPos + (*dirForward * radStraight); // v88

        // Forward arc across [-angleStraight, +angleStraight].
        int nFwd = 1 - (int)((angleStraight / step_1) * -2.0f);
        for (int i = 0; i < nFwd; ++i)
        {
            float a = i * step_1 - angleStraight;
            if (a > angleStraight)
                a = angleStraight;
            m3dRotatePointAroundRay(startPos, &sliceAxis, a, &fwdPoint, &rotated);
            sliceOutline.PushBack(rotated);
        }
        m3dRotatePointAroundRay(startPos, &sliceAxis, -angleStraight, &fwdPoint, &rotated);
        ringFwdLow.PushBack(rotated);
        m3dRotatePointAroundRay(startPos, &sliceAxis, angleStraight, &fwdPoint, &rotated);
        ringFwdHigh.PushBack(rotated);

        // Peripheral spoke point again (radSide) for the +angle side.
        m3dV sidePoint2 = *startPos + (*dirForward * radSide); // v89
        m3dRotatePointAroundRay(startPos, &sliceAxis, angleStraight, &sidePoint2, &rotated);
        ringPeriphHigh.PushBack(rotated);

        // Peripheral arc from +angleStraight up to +angleSide.
        int nSide2 = (int)((angleSide - angleStraight) / step_1);
        for (int i = 0; i <= nSide2; ++i)
        {
            float a = i * step_1 + angleStraight;
            if (a > angleSide)
                a = angleSide;
            m3dRotatePointAroundRay(startPos, &sliceAxis, a, &sidePoint2, &rotated);
            sliceOutline.PushBack(rotated);
        }
        m3dRotatePointAroundRay(startPos, &sliceAxis, angleSide, &sidePoint2, &rotated);
        ringSideHigh.PushBack(rotated);

        sliceOutline.PushBack(*startPos);
        visCone->PushBack(sliceOutline);
    }

    // Fold each "High" rib into its "Low" rib, close the loop, and append the ring.
    for (int i = 0; i < ringSideHigh.nElem; ++i)
        ringSideLow.PushBack(ringSideHigh[i]);
    if (ringSideLow.nElem)
        ringSideLow.PushBack(ringSideLow[0]);
    visCone->PushBack(ringSideLow);

    for (int i = 0; i < ringPeriphHigh.nElem; ++i)
        ringPeriphLow.PushBack(ringPeriphHigh[i]);
    if (ringPeriphLow.nElem)
        ringPeriphLow.PushBack(ringPeriphLow[0]);
    visCone->PushBack(ringPeriphLow);

    for (int i = 0; i < ringFwdHigh.nElem; ++i)
        ringFwdLow.PushBack(ringFwdHigh[i]);
    if (ringFwdLow.nElem)
        ringFwdLow.PushBack(ringFwdLow[0]);
    visCone->PushBack(ringFwdLow);
}
