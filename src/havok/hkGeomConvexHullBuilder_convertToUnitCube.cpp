/* ?convertToUnitCube@hkGeomConvexHullBuilder@@SAXAAV?$hkArray@VhkVector4@@@@AAVhkVector4@@1@Z @0x83FD96C8 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkGeomConvexHullBuilder::convertToUnitCube
   Rescales all vertices into a unit-sized box centred at the origin, and reports the
   applied scale (AABB extents) and offset (AABB centre) so convertFromUnitCube can undo it.
   For each axis: out = (in - centre) / extent, using extent==0 (<= FLT_EPSILON) -> reciprocal
   1.0 to leave a degenerate axis untouched. The w-lane is cleared (the diagonal scale matrix
   has zeroed off-diagonal/w columns, so the transformed w is 0). DEVIATION: the original
   builds a hkMatrix3 diagonal and calls hkMatrix3::mul per vertex; that reduces to the
   per-component scale reproduced here. Reciprocal via 1.0f/x (original: VMX estimate+Newton). */
void hkGeomConvexHullBuilder_convertToUnitCube(hkArray<hkVector4> *verts, hkVector4 *scaleOut, hkVector4 *offsetOut)
{
    const float EPS = 1.1920929e-07f; /* FLT_EPSILON */
    hkAabb aabb;
    float recip[3];
    int i, axis;

    hkGeomConvexHullBuilder_getAabb(verts, &aabb);

    for (axis = 0; axis < 3; ++axis)
    {
        float extent = aabb.m_max.m_quad.___u0.v[axis] - aabb.m_min.m_quad.___u0.v[axis];
        scaleOut->m_quad.___u0.v[axis] = extent;
        offsetOut->m_quad.___u0.v[axis] =
            (aabb.m_max.m_quad.___u0.v[axis] + aabb.m_min.m_quad.___u0.v[axis]) * 0.5f;
        recip[axis] = (extent <= EPS) ? 1.0f : (1.0f / extent);
    }
    scaleOut->m_quad.___u0.v[3] = 1.0f;

    for (i = 0; i < verts->m_size; ++i)
    {
        hkVector4 *v = &verts->m_data[i];
        for (axis = 0; axis < 3; ++axis)
            v->m_quad.___u0.v[axis] =
                (v->m_quad.___u0.v[axis] - offsetOut->m_quad.___u0.v[axis]) * recip[axis];
        v->m_quad.___u0.v[3] = 0.0f;
    }
}
