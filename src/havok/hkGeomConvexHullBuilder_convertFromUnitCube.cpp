/* ?convertFromUnitCube@hkGeomConvexHullBuilder@@SAXAAV?$hkArray@VhkVector4@@@@AAVhkVector4@@1@Z @0x83FD98A0 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkGeomConvexHullBuilder::convertFromUnitCube
   Inverse of convertToUnitCube: maps each unit-cube vertex back to world space via
   out = in * scale + offset, per axis. DEVIATION: the original builds a hkMatrix3 diagonal
   from scale and calls hkMatrix3::mul per vertex, then vector-adds offset; that reduces to
   the per-component scale+add reproduced here. The scale matrix's w column is zero, so the
   transformed w is 0 before the add, leaving out.w == offset.w. */
void hkGeomConvexHullBuilder_convertFromUnitCube(hkArray<hkVector4> *verts, hkVector4 *scale, hkVector4 *offset)
{
    int i, axis;

    for (i = 0; i < verts->m_size; ++i)
    {
        hkVector4 *v = &verts->m_data[i];
        for (axis = 0; axis < 3; ++axis)
            v->m_quad.___u0.v[axis] =
                v->m_quad.___u0.v[axis] * scale->m_quad.___u0.v[axis] + offset->m_quad.___u0.v[axis];
        v->m_quad.___u0.v[3] = offset->m_quad.___u0.v[3];
    }
}
