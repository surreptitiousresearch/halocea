// lgtLIGHT__GetOBB @0x82A79E48
#include <math.h>
#include "../../headers/ws/lgt/lgtLIGHT.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dBOX.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// lgtLIGHT::GetOBB @ 0x82A79E5C  (?GetOBB@lgtLIGHT@@QAAHPAVm3dOBB@@@Z)
//
// Compute the light's bounding oriented box into `*pOBB`. Returns 0 when the light has no
// attenuation volume (atten == 0); otherwise 1. The box depends on the light type:
//   type 0 (omni)   : axis-aligned cube of half-extent range about the light origin.
//   type 1 (spot)   : cone approximated by a box whose cross-section is 2*tan(halfFov)*range.
//   type 3 (linear) : box sized by fallOffAngle (used here as the tube half-length) x range.
//   other types     : returned as-is (box left to the caller's prior contents).
//
// `range` = scale * scaleDeg * attenEnd is the light's world-space reach.
//
// ABI: the mangling has one param (m3dOBB*); the decompiler's extra `long double a3` is a stack
// scratch for the tan() call and is dropped.

static const float kDegToRad = 0.017453292f;

int lgtLIGHT::GetOBB(m3dOBB *pOBB)
{
    if (!atten)
        return 0;

    if (type == 0) {
        // Omni: AABB cube centred on the origin, expanded by the reach on every axis.
        float range = scaleDeg * scale;
        m3dBOX box;
        box.fur.x = matrL2W.elements[12];
        box.fur.y = matrL2W.elements[13];
        box.fur.z = matrL2W.elements[14];
        box.bll = box.fur;
        box.Expand(range * attenEnd);
        box.ConvToOBB(pOBB);
        return 1;
    }

    if (type == 1) {
        // Spot: half cross-section = clamp(tan(fallOffAngle/2 deg), <=1) * range.
        float range = scale * scaleDeg * attenEnd;
        float t = (float)tan(fallOffAngle * 0.5f * kDegToRad);
        if (t > 1.0f)
            t = 1.0f;
        float cross = t * range;
        pOBB->CalcStart((m3dV *)&matrL2W, &matrL2W.v[1], &matrL2W.v[2]);
        _m3dLineCombine(&matrL2W.v[3], (const m3dV *)&matrL2W, cross * -0.5f, &pOBB->org);
        _m3dLineCombine(&pOBB->org, &matrL2W.v[1], cross * -0.5f, &pOBB->org);
        pOBB->sx = cross;
        pOBB->sy = cross;
        pOBB->sz = scale * scaleDeg * attenEnd;
        return 1;
    }

    if (type == 3) {
        // Linear: cross-section 2*fallOffAngle, length = range.
        pOBB->CalcStart((m3dV *)&matrL2W, &matrL2W.v[1], &matrL2W.v[2]);
        _m3dLineCombine(&matrL2W.v[3], (const m3dV *)&matrL2W, -fallOffAngle, &pOBB->org);
        _m3dLineCombine(&pOBB->org, &matrL2W.v[1], -fallOffAngle, &pOBB->org);
        pOBB->sx = fallOffAngle * 2.0f;
        pOBB->sy = fallOffAngle * 2.0f;
        pOBB->sz = scaleDeg * scale * attenEnd;
        return 1;
    }

    // Other light types: leave pOBB as-is.
    return 1;
}
