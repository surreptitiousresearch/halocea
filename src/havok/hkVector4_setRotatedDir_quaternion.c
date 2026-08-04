#include "../headers/havok/hkVector4.h"

/* hkVector4::setRotatedDir(hkQuaternion const &, hkVector4 const &) @0x83879D30
   ?setRotatedDir@hkVector4@@QAAXABVhkQuaternion@@ABV1@@Z

   RENAMED (was hkVector4_setRotatedDir): setRotatedDir is overloaded, and the bare name was
   bound to two different binary functions at once. Every corpus call site — inverseTransformPlane
   (0x83F94A90), calculateSemiAccurateConnectivity (0x83A49EA0), decorateConnection,
   transformVerticesAndPlaneEquations, createCylinderConvexVerticesShape — targets
   ?_setRotatedDir@hkVector4@@QAAXABVhkRotation@@ABV1@@Z @0x83601070 instead, which is a plain
   matrix-vector multiply (three lvx128 columns off r4 at +0/+0x10/+0x20, vspltw of the direction's
   lanes, two vmaddfp). That one stays a boundary extern; nothing calls the quaternion overload
   below, so leaving them to share a name would have pointed all six at the wrong algorithm.

   hkVector4::setRotatedDir — rotate a direction vector by a quaternion.
   DEVIATION: original is VMX128 (vmsum3fp128 dot, vpermwi128 cross-product
   shuffles, a fused chain against hkQuadRealMinusHalf, doubled at the end).
   Reconstructed here as scalar float that reproduces the exact per-lane fused
   sequence the codegen performs:
       cross = quat.xyz x direction        (w lane = 0)
       dot   = quat.xyz . direction
       half  = 0.5 * quat.w
       t     = dot * (half * direction) + quat
       this  = 2 * (quat.w * t + cross) */
void hkVector4_setRotatedDir_quaternion(hkVector4 *this, const hkVector4 *quat, const hkVector4 *direction)
{
    float qx = quat->m_quad.___u0.v[0], qy = quat->m_quad.___u0.v[1];
    float qz = quat->m_quad.___u0.v[2], qw = quat->m_quad.___u0.v[3];
    float dx = direction->m_quad.___u0.v[0], dy = direction->m_quad.___u0.v[1];
    float dz = direction->m_quad.___u0.v[2], dw = direction->m_quad.___u0.v[3];
    float dot  = qx*dx + qy*dy + qz*dz;
    float half = 0.5f * qw;
    float cross[4];
    float q[4], d[4];
    int i;
    cross[0] = qy*dz - qz*dy;
    cross[1] = qz*dx - qx*dz;
    cross[2] = qx*dy - qy*dx;
    cross[3] = 0.0f;
    q[0] = qx; q[1] = qy; q[2] = qz; q[3] = qw;
    d[0] = dx; d[1] = dy; d[2] = dz; d[3] = dw;
    for (i = 0; i < 4; ++i)
    {
        float t = dot * (half * d[i]) + q[i];
        this->m_quad.___u0.v[i] = 2.0f * (qw * t + cross[i]);
    }
}
