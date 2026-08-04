#include "../headers/havok/hkpConvexVerticesShape.h"

extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

/* hkpConvexVerticesShape::getOriginalVertices @ 0x8360FB98
   Unpack the SoA rotated-vertex store back into a plain hkArray<hkVector4>.
   DEVIATION: the source packs four vertices per FourVectors block (xxxx yyyy
   zzzz); the VMX lvx/stvx copy per vertex is reconstructed as scalar lane reads.
   The w lane is left as written by the original (0). */
void hkpConvexVerticesShape_getOriginalVertices(hkpConvexVerticesShape *self, hkArray<hkVector4> *vertices)
{
    int n = self->m_numVertices;
    int cap = vertices->m_capacityAndFlags & 0x3FFFFFFF;
    int i;

    if (cap < n)
    {
        int grow = 2 * cap;
        if (n >= grow)
            grow = self->m_numVertices;
        hkArrayUtil__reserve(&vertices->m_data, grow, 16);
    }

    vertices->m_size = n;
    for (i = 0; i < self->m_numVertices; ++i)
    {
        hkpConvexVerticesShape_FourVectors *block = &self->m_rotatedVertices.m_data[i / 4];
        int lane = i & 3;
        vertices->m_data[i].m_quad.___u0.vector4_f32[0] = block->m_x.m_quad.___u0.vector4_f32[lane];
        vertices->m_data[i].m_quad.___u0.vector4_f32[1] = block->m_y.m_quad.___u0.vector4_f32[lane];
        vertices->m_data[i].m_quad.___u0.vector4_f32[2] = block->m_z.m_quad.___u0.vector4_f32[lane];
        vertices->m_data[i].m_quad.___u0.vector4_f32[3] = 0.0f;
    }
}
