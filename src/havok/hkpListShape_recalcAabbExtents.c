#include "../headers/havok/hkpListShape.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hkTransform.h"

/* boundary — the shared identity transform used to query children in list space. */
extern const hkTransform hkTransformIdentity;

/* boundary — non-virtual forwarder to the child shape's virtual getAabbImpl. */
extern void hkpShape_getAabb(const hkpShape *self, const hkTransform *localToWorld,
                             float tolerance, hkAabb *out); /* hkpShape::getAabb */

/* hkpListShape::recalcAabbExtents — recompute the cached local-space AABB of the
   list from its children and, from that, the cached half-extents/center. The
   first child's AABB seeds `aabb`; each subsequent child's AABB (queried in the
   identity frame, zero tolerance) is merged component-wise via min/max. The
   half-extents and center are then (max-/+min) * 0.5.

   DEVIATION: original is VMX128 (vminfp/vmaxfp/vsubfp/vaddfp with a splatted 0.5);
   reconstructed as scalar per-lane loops over all four hkVector4 lanes. */
void hkpListShape_recalcAabbExtents(hkpListShape *self, hkAabb *aabb)
{
    hkAabb childAabb;
    int i, lane;

    hkpShape_getAabb(self->m_childInfo.m_data[0].m_shape, &hkTransformIdentity, 0.0f, aabb);

    for (i = 1; i < self->m_childInfo.m_size; ++i)
    {
        hkpShape_getAabb(self->m_childInfo.m_data[i].m_shape, &hkTransformIdentity, 0.0f, &childAabb);
        for (lane = 0; lane < 4; ++lane)
        {
            float lo = aabb->m_min.m_quad.___u0.v[lane];
            float hi = aabb->m_max.m_quad.___u0.v[lane];
            float clo = childAabb.m_min.m_quad.___u0.v[lane];
            float chi = childAabb.m_max.m_quad.___u0.v[lane];
            aabb->m_min.m_quad.___u0.v[lane] = (clo < lo) ? clo : lo;
            aabb->m_max.m_quad.___u0.v[lane] = (chi > hi) ? chi : hi;
        }
    }

    for (lane = 0; lane < 4; ++lane)
    {
        float lo = aabb->m_min.m_quad.___u0.v[lane];
        float hi = aabb->m_max.m_quad.___u0.v[lane];
        self->m_aabbHalfExtents[lane] = (hi - lo) * 0.5f;
        self->m_aabbCenter[lane]      = (hi + lo) * 0.5f;
    }
}
