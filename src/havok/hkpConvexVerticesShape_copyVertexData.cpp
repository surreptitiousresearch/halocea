#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hkTransform.h"

extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

/* hkpAabbUtil::calcAabb — real 5-arg DEF (transform, halfExtents, radius, reserved, out). */
extern void hkpAabbUtil_calcAabb(const hkTransform *localToWorld, const hkVector4 *halfExtents, float extraRadius, void *_reservedR5, hkAabb *aabbOut); /* hkpAabbUtil::calcAabb */

static void storeVertex(hkpConvexVerticesShape_FourVectors *blocks, int index,
                        float x, float y, float z)
{
    hkpConvexVerticesShape_FourVectors *b = &blocks[index / 4];
    int lane = index & 3;
    b->m_x.m_quad.___u0.vector4_f32[lane] = x;
    b->m_y.m_quad.___u0.vector4_f32[lane] = y;
    b->m_z.m_quad.___u0.vector4_f32[lane] = z;
}

/* hkpConvexVerticesShape::copyVertexData @ 0x8360FFD8
   Pack `numVertices` strided input positions into the SoA rotated-vertex store
   (four vertices per FourVectors block), replicating the last vertex into the
   trailing padding lanes so every lane holds a valid point, then cache the AABB.
   DEVIATION: the binary heavily unrolls the SoA scatter and the final VMX AABB
   center/half-extents math; both are reconstructed as straight-line scalar C. */
void hkpConvexVerticesShape_copyVertexData(hkpConvexVerticesShape *self, const float *vertexIn,
                                           int byteStriding, int numVertices)
{
    hkpConvexVerticesShape_FourVectors *blocks;
    int paddedCount = (numVertices + 3) & ~3;
    int numBlocks = paddedCount >> 2;
    int cap;
    int i;
    hkAabb aabb;
    float minX, minY, minZ, maxX, maxY, maxZ;

    self->m_numVertices = numVertices;
    cap = self->m_rotatedVertices.m_capacityAndFlags & 0x3FFFFFFF;
    if (cap < numBlocks)
    {
        int grow = 2 * cap;
        if (numBlocks >= grow)
            grow = paddedCount >> 2;
        hkArrayUtil__reserve(&self->m_rotatedVertices.m_data, grow, 48);
    }
    self->m_rotatedVertices.m_size = numBlocks;
    blocks = self->m_rotatedVertices.m_data;

    for (i = 0; i < numVertices; ++i)
    {
        const float *v = (const float *)((const char *)vertexIn + (size_t)byteStriding * i);
        storeVertex(blocks, i, v[0], v[1], v[2]);
    }

    /* Replicate the final real vertex into the padding lanes. */
    if (numVertices > 0 && paddedCount > numVertices)
    {
        const float *last = (const float *)((const char *)vertexIn + (size_t)byteStriding * (numVertices - 1));
        for (i = numVertices; i < paddedCount; ++i)
            storeVertex(blocks, i, last[0], last[1], last[2]);
    }

    /* DEVIATION: synced to the 5-arg DEF (transform, halfExtents, radius, reserved,
       out): the aabb-out moves to arg5 with a NULL reserved arg4, and the leading
       reconstruction values are reinterpreted into the typed parameter slots. */
    hkpAabbUtil_calcAabb((const hkTransform *)vertexIn, (const hkVector4 *)&numVertices,
                         (float)byteStriding, (void *)0, &aabb);
    minX = aabb.m_min.m_quad.___u0.vector4_f32[0];
    minY = aabb.m_min.m_quad.___u0.vector4_f32[1];
    minZ = aabb.m_min.m_quad.___u0.vector4_f32[2];
    maxX = aabb.m_max.m_quad.___u0.vector4_f32[0];
    maxY = aabb.m_max.m_quad.___u0.vector4_f32[1];
    maxZ = aabb.m_max.m_quad.___u0.vector4_f32[2];

    self->m_aabbCenter.m_quad.___u0.vector4_f32[0] = (minX + maxX) * 0.5f;
    self->m_aabbCenter.m_quad.___u0.vector4_f32[1] = (minY + maxY) * 0.5f;
    self->m_aabbCenter.m_quad.___u0.vector4_f32[2] = (minZ + maxZ) * 0.5f;
    self->m_aabbHalfExtents.m_quad.___u0.vector4_f32[0] = (maxX - minX) * 0.5f;
    self->m_aabbHalfExtents.m_quad.___u0.vector4_f32[1] = (maxY - minY) * 0.5f;
    self->m_aabbHalfExtents.m_quad.___u0.vector4_f32[2] = (maxZ - minZ) * 0.5f;
}
