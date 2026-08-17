/* ?calcAabb@hkpAabbUtil@@SAXABVhkTransform@@ABVhkVector4@@MAAVhkAabb@@@Z @0x836027A8 */
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkAabb.h"

/* hkpAabbUtil::calcAabb — world-space AABB of an oriented box. The box is defined
   by its local half-extents and the rigid transform localToWorld; extraRadius is
   an isotropic inflation added to every axis. The world extent along each axis is
   |R| * halfExtents (absolute value of each rotation column scaled by the matching
   half-extent, summed) plus extraRadius; the AABB is translation +/- that extent.

   DEVIATION: original is VMX128; reconstructed as scalar loops over 4 lanes.
   DEVIATION: the ABI has an unused pointer slot in r5 — the output AABB is passed
   in r6 (disasm 0x8360282C/0x83602830 store to r6), and extraRadius arrives as a
   scalar float in f1 (stfs f1 at 0x836027E4). Reproduced with an explicit
   _reservedR5 placeholder so the register assignment stays faithful. */
void hkpAabbUtil_calcAabb(const hkTransform *localToWorld, const hkVector4 *halfExtents,
                          float extraRadius, void *_reservedR5, hkAabb *aabbOut)
{
    const hkVector4 *cols[3] = { &localToWorld->m_rotation.m_col0,
                                 &localToWorld->m_rotation.m_col1,
                                 &localToWorld->m_rotation.m_col2 };
    const float *t = localToWorld->m_translation.m_quad.___u0.v;
    int r;

    (void)_reservedR5;

    for (r = 0; r < 4; ++r)
    {
        float extent = 0.0f;
        int c;
        for (c = 0; c < 3; ++c)
        {
            float term = halfExtents->m_quad.___u0.v[c] * cols[c]->m_quad.___u0.v[r];
            extent += (term < 0.0f) ? -term : term;   /* setAbs4 positiveMask */
        }
        extent += extraRadius;
        aabbOut->m_min.m_quad.___u0.v[r] = t[r] - extent;
        aabbOut->m_max.m_quad.___u0.v[r] = t[r] + extent;
    }
}
