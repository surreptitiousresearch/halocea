#include "../headers/havok/hkpShapeVtbl.h"
#include "../headers/havok/hkVector4.h"

#ifndef HK_MATH_FABSF
#define HK_MATH_FABSF(x) ((x) < 0.0f ? -(x) : (x))
#endif

/* hkpShape::getMaximumProjection — the extent of the shape's world AABB along a
   direction. It queries the shape's own AABB (identity transform, zero
   tolerance) via the virtual getAabbImpl, then returns the box's supporting
   value along `direction`:
       dot(center, d) + dot(halfExtents, |d|)
   with center = (max+min)/2 and halfExtents = (max-min)/2.
   DEVIATION: the original is fully VMX128 (vaddfp/vsubfp of the aabb corners,
   sign-copy of the direction via hkVector4Util::mulSigns4, vmaddfp, vmsum3fp128
   3-lane reduction); reconstructed as the equivalent scalar formula over xyz. */
float hkpShape_getMaximumProjection(hkpShape *self, const hkVector4 *direction)
{
    hkAabb aabb;
    hkTransform identity;
    float result = 0.0f;
    int i;

    /* identity localToWorld (basis I, zero translation) */
    for (i = 0; i < 3; ++i)
    {
        int j;
        for (j = 0; j < 4; ++j)
            (&identity.m_rotation.m_col0)[i].m_quad.___u0.v[j] = (i == j) ? 1.0f : 0.0f;
    }
    for (i = 0; i < 4; ++i)
        identity.m_translation.m_quad.___u0.v[i] = 0.0f;

    HKP_SHAPE_VTBL(self)->getAabbImpl(self, &identity, &aabb, 0.0f);

    for (i = 0; i < 3; ++i)
    {
        float mn = aabb.m_min.m_quad.___u0.v[i];
        float mx = aabb.m_max.m_quad.___u0.v[i];
        float center = 0.5f * (mx + mn);
        float halfExtent = 0.5f * (mx - mn);
        float d = direction->m_quad.___u0.v[i];
        result += center * d + halfExtent * HK_MATH_FABSF(d);
    }
    return result;
}
