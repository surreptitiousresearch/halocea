#include <math.h>
#include "../../headers/ws/lgt/lgtLIGHT.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// lgtLIGHT::CalcAttenuation @ 0x82A7A1EC  (?CalcAttenuation@lgtLIGHT@@QBAMPAUm3dV@@@Z)
//
// Evaluate this light's attenuation at world position `point`, in [0,1] (0 = out of range /
// no contribution, 1 = full). The result is the product of a distance term (shaped by the
// `atten` curve selector) and, for spot (type 1) / linear (type 3) lights, an angular term.
//
//   type 2 : pure-ambient light -> always full (1.0), distance-independent.
//   atten 0: no distance falloff (distance term = 1), only angular shaping applies.
//   type 4 : cube light -> per-axis CalcCubeAttenuation, taking the tightest axis.
//   else   : linear range remap between attenStart and attenEnd (scaled by scale*scaleDeg).
//
// `scale` and `scaleDeg` together form the world/units range scale applied to the atten radii.

// Distance from `point` to the segment (lineP1..lineP2), closest point written to `*outClosest`
// (?m3dDistPointLine@@YAMPBUm3dV@@00PAU1@@Z).  boundary
extern float m3dDistPointLine(const m3dV *point, const m3dV *lineP1, const m3dV *lineP2,
                              m3dV *outClosest);

static const float kDistEpsilon   = 0.000001f;

float lgtLIGHT::CalcAttenuation(m3dV *point) const
{
    if (type == 2)
        return 1.0f; // pure-ambient: no spatial attenuation

    // Vector from `point` to the light origin, and its squared length.
    m3dV delta;
    delta.x = matrL2W.elements[12] - point->x;
    delta.y = matrL2W.elements[13] - point->y;
    delta.z = matrL2W.elements[14] - point->z;
    float dist2 = m3dLengthVector_2(&delta);
    if (fabsf(dist2) < kDistEpsilon)
        return 1.0f; // at the light origin

    const float rangeScale = scaleDeg * scale;

    float distAtten = 1.0f;
    float dist;

    if (!atten) {
        // No distance falloff: distance term stays 1, only angular shaping applies below.
        _m3dCheckValid(dist2);
        dist = sqrtf(dist2);
    } else {
        // Cull beyond the (scaled) end radius.
        float endRadius = attenEnd * attenEnd * scale * scale * scaleDeg * scaleDeg;
        if (dist2 > endRadius)
            return 0.0f;

        _m3dCheckValid(dist2);
        dist = sqrtf(dist2);

        float shaped;
        if (type == 4) {
            // Cube light: transform delta into light space, attenuate each axis, take the min.
            float lx = -( delta.x * matrL2W.elements[0]
                        + matrL2W.elements[2] * delta.z
                        + matrL2W.elements[1] * delta.y );
            float ly = -( delta.x * matrL2W.elements[4]
                        + matrL2W.elements[6] * delta.z
                        + matrL2W.elements[5] * delta.y );
            float lz =    matrL2W.elements[8]  * delta.x
                        + matrL2W.elements[10] * delta.z
                        + matrL2W.elements[9]  * delta.y;

            float ax = lgtLIGHT__CalcCubeAttenuation(lx, cubeAttenEnd.x, cubeAttenStartPos.x, cubeAttenStartNeg.x);
            float ay = lgtLIGHT__CalcCubeAttenuation(ly, cubeAttenEnd.y, cubeAttenStartPos.y, cubeAttenStartNeg.y);
            float az = lgtLIGHT__CalcCubeAttenuation(lz, cubeAttenEnd.z, cubeAttenStartPos.z, cubeAttenStartNeg.z);
            shaped = az;
            if (ay < shaped) shaped = ay;
            if (ax < shaped) shaped = ax;
        } else {
            shaped = m3dClampZero(1.0f, 0.0f, attenStart * rangeScale, rangeScale * attenEnd, dist);
        }
        distAtten = shaped;

        // Apply the atten curve (selectors 2..4; anything else falls to the inverse-square form).
        if ((unsigned int)(atten - 2) <= 3) {
            switch (atten) {
            case 2:
                _m3dCheckValid(distAtten);
                distAtten = distAtten * distAtten;
                break;
            case 3:
                _m3dCheckValid(distAtten);
                distAtten = distAtten * distAtten * distAtten;
                break;
            case 4: {
                float a, b, c;
                lgtLIGHT::GetLightDistAttenCoeffs(&b, &a, &c,
                                                  rangeScale * attenEnd,
                                                  attenStart * rangeScale,
                                                  false);
                float v = b / (a * dist2 + 1.0f) + c;
                distAtten = v * v;
                break;
            }
            default: {
                // Inverse-square falloff scaled by the end radius.
                float r = attenEnd * scaleDeg * scale;
                float v = 0.0f / ((-1.0f / (r * r)) * dist2 + 1.0f);
                distAtten = v * v;
                break;
            }
            }
        }
    }

    // Angular shaping by light type. `result` starts at the full 1.0 factor.
    float result = 1.0f;
    if (type == 1) {
        // Spot: angle between the (normalised) light->point vector and the light's -Z axis.
        _m3dScaleVector(&delta, 1.0f / dist);
        m3dV axis;
        axis.x = matrL2W.elements[8];
        axis.y = matrL2W.elements[9];
        axis.z = matrL2W.elements[10];
        _m3dCheckValid(&axis);
        _m3dNegateVector(&axis);
        float angle = m3dAngleVector(&delta, &axis);
        float fallOff = fallOffAngle * 0.5f;
        float hotSpot = hotSpotAngle * 0.5f;
        result = m3dClampZero(1.0f, 0.0f, hotSpot, fallOff, angle);
    } else if (type == 3) {
        // Linear: distance from `point` to the light's axis line (origin along its Z axis).
        m3dV axis;
        axis.x = matrL2W.elements[8];
        axis.y = matrL2W.elements[9];
        axis.z = matrL2W.elements[10];
        _m3dCheckValid(&axis);
        // DEVIATION: the decompiler scrambles the line's second endpoint / distance output regs
        // here (uninitialised v24/v36 aliases). Reconstructed as the segment from the light
        // origin along its axis, with the 4-arg m3dDistPointLine writing the closest point out.
        m3dV lineP2;
        _m3dLineCombine(&matrL2W.v[3], &axis, 1.0f, &lineP2);
        m3dV closest;
        float lineDist = m3dDistPointLine(point, &matrL2W.v[3], &lineP2, &closest);
        float fallOff = fallOffAngle;
        float hotSpot = hotSpotAngle;
        result = m3dClampZero(1.0f, 0.0f, hotSpot, fallOff, lineDist);
    }
    // type 0 (omni): no angular term.

    return result * distAtten;
}
