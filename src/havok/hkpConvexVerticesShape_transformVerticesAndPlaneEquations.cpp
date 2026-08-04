#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkRotation.h"
#include "../headers/havok/hkThreadMemory.h"

/* Rotate a direction vector by a rotation matrix, writing the result into out.
   Boundary: hkVector4::setRotatedDir. */
extern void hkVector4_setRotatedDir(hkVector4 *out, const hkRotation *rotation, const hkVector4 *dir); /* hkVector4::setRotatedDir */

/* hkpConvexVerticesShape::transformVerticesAndPlaneEquations
   Applies the rigid transform t to the shape's geometry in place: every vertex is
   rotated + translated, and every plane equation has its normal rotated and its
   distance term corrected for the translation.

   DEVIATION: the original is entirely VMX128; the vector register math is
   reconstructed here as scalar operations over the four float lanes
   (m_quad.___u0.v[]). The per-thread bump/pop scratch allocator (r13 TLS deref +
   704, inlined at every call site) is expressed through the hkThreadMemory
   stack-allocation API. */
void hkpConvexVerticesShape_transformVerticesAndPlaneEquations(hkpConvexVerticesShape *self,
                                                               const hkTransform *t)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    const hkVector4 *col0 = &t->m_rotation.m_col0;
    const hkVector4 *col1 = &t->m_rotation.m_col1;
    const hkVector4 *col2 = &t->m_rotation.m_col2;
    const hkVector4 *trans = &t->m_translation;

    int numSpheres = hkpConvexVerticesShape_getNumCollisionSpheresImpl(self);

    hkArray<hkVector4> vertices;
    hkVector4 *scratchBase;
    int i;

    vertices.m_data = (hkVector4 *)hkThreadMemory_allocateStack(mem, (numSpheres + 1) * 16);
    vertices.m_size = 0;
    vertices.m_capacityAndFlags = numSpheres | 0x80000000; /* stack-owned marker */
    scratchBase = vertices.m_data;

    hkpConvexVerticesShape_getOriginalVertices(self, &vertices);

    /* Transform each original vertex: p' = col0*x + col1*y + col2*z + translation. */
    for (i = vertices.m_size - 1; i >= 0; --i)
    {
        hkVector4 *v = &vertices.m_data[i];
        float x = v->m_quad.___u0.v[0];
        float y = v->m_quad.___u0.v[1];
        float z = v->m_quad.___u0.v[2];
        int lane;
        for (lane = 0; lane < 4; ++lane)
        {
            v->m_quad.___u0.v[lane] =
                col0->m_quad.___u0.v[lane] * x +
                col1->m_quad.___u0.v[lane] * y +
                col2->m_quad.___u0.v[lane] * z +
                trans->m_quad.___u0.v[lane];
        }
    }

    hkpConvexVerticesShape_copyVertexData(self, vertices.m_data->m_quad.___u0.vector4_f32, 16, vertices.m_size); /* union view name per __vector4 DB layout */

    /* Build the rotation matrix and the translation-dot vector used to correct the
       plane distances. negTrans = -translation (the VMX `vxor` with negateMask flips
       the float sign bits). transDot lanes = dot3(colK, -translation). */
    {
        hkRotation rotation;
        float negTrans0 = -trans->m_quad.___u0.v[0];
        float negTrans1 = -trans->m_quad.___u0.v[1];
        float negTrans2 = -trans->m_quad.___u0.v[2];
        float transDot0, transDot1, transDot2;
        int p;

        rotation.m_col0 = *col0;
        rotation.m_col1 = *col1;
        rotation.m_col2 = *col2;

        transDot0 = col0->m_quad.___u0.v[0] * negTrans0 + col0->m_quad.___u0.v[1] * negTrans1 + col0->m_quad.___u0.v[2] * negTrans2;
        transDot1 = col1->m_quad.___u0.v[0] * negTrans0 + col1->m_quad.___u0.v[1] * negTrans1 + col1->m_quad.___u0.v[2] * negTrans2;
        transDot2 = col2->m_quad.___u0.v[0] * negTrans0 + col2->m_quad.___u0.v[1] * negTrans1 + col2->m_quad.___u0.v[2] * negTrans2;

        for (p = self->m_planeEquations.m_size - 1; p >= 0; --p)
        {
            hkVector4 *plane = &self->m_planeEquations.m_data[p];
            float oldW = plane->m_quad.___u0.v[3];
            float distCorrection = plane->m_quad.___u0.v[0] * transDot0 +
                                   plane->m_quad.___u0.v[1] * transDot1 +
                                   plane->m_quad.___u0.v[2] * transDot2;
            hkVector4_setRotatedDir(plane, &rotation, plane); /* normal := R * normal (in place) */
            plane->m_quad.___u0.v[3] = oldW + distCorrection;
        }
    }

    hkThreadMemory_deallocateStack(mem, scratchBase);
}
