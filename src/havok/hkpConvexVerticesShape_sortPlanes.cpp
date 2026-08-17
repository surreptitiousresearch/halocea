/* ?sortPlanes@hkpConvexVerticesShape@@IAAXXZ @0x836108A8 */
#include <math.h>
#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkThreadMemory.h"

/* Level-1 boundary callees (bounded descent — kept extern). */
extern void hkpConvexVerticesShape_getOriginalVertices(hkpConvexVerticesShape *shape, hkArray<hkVector4> *verticesOut); /* hkpConvexVerticesShape::getOriginalVertices */
extern void hkArray_hkVector4_spliceInto(hkArray<hkVector4> *self, int index, int numToRemove,
                                         const hkVector4 *insertData, int numToInsert); /* hkArray<hkVector4>::spliceInto */

/* hkpConvexVerticesShape::sortPlanes — reorder m_planeEquations so that plane i is
   the one belonging to face i of the shape's connectivity. The existing planes are
   moved to a scratch pool; the connectivity is then walked face by face. For each
   face the outward normal is the (unnormalized) cross product of the edges out of
   its first vertex; the pool plane whose normal best matches (largest dot product)
   is appended to m_planeEquations and swap-removed from the pool. Degenerate faces
   (zero-area, so no usable normal) append the pool's leading plane as a filler.
   Any planes still left in the pool afterwards are appended in bulk.

   DEVIATION: original is dense VMX128 (vpermwi cross products, vmsum3fp dot
   products, vrsqrtefp-style reciprocal via 1/__fsqrts, and quadword element
   copies). Reconstructed as scalar 3-component math over the hkVector4 lanes; the
   scratch pool uses the thread chunk allocator and the vertex buffer the thread
   stack allocator, exactly as the original. */
void hkpConvexVerticesShape_sortPlanes(hkpConvexVerticesShape *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    const hkpConvexVerticesConnectivity *conn;
    hkArray<hkVector4> vertices;
    hkVector4 *savedPlanes = nullptr;
    int savedCapMarker = (int)0x80000000;
    int poolCount;                 /* planes still in the scratch pool */
    int numVerts;
    int faceIndex;
    int vertexBase;                /* running index into m_vertexIndices */
    int i;

    /* move the current plane equations into the scratch pool */
    poolCount = self->m_planeEquations.m_size;
    if (poolCount > 0)
    {
        savedPlanes = (hkVector4 *)hkThreadMemory_allocateChunk(mem, 16 * poolCount, HK_MEMORY_CLASS_ARRAY);
        poolCount = self->m_planeEquations.m_size;
        savedCapMarker = poolCount;
        for (i = 0; i < poolCount; ++i)
            savedPlanes[i] = self->m_planeEquations.m_data[i];
    }
    self->m_planeEquations.m_size = 0;

    /* original vertices into a thread-stack scratch array */
    numVerts = self->m_numVertices;
    vertices.m_data = (hkVector4 *)hkThreadMemory_allocateStack(mem, 16 * (numVerts + 1));
    vertices.m_size = 0;
    vertices.m_capacityAndFlags = numVerts | (int)0x80000000;
    hkpConvexVerticesShape_getOriginalVertices(self, &vertices);

    conn = self->m_connectivity;
    if (conn->m_numVerticesPerFace.m_size > 0)
    {
        vertexBase = 0;
        faceIndex = 0;
        while (poolCount)
        {
            int faceVertCount = conn->m_numVerticesPerFace.m_data[faceIndex];
            if (faceVertCount)
            {
                const unsigned short *idx = &conn->m_vertexIndices.m_data[vertexBase];
                int i0 = idx[0], i1 = idx[1], i2 = idx[2];
                const float *p0 = vertices.m_data[i0].m_quad.___u0.v;
                const float *p1 = vertices.m_data[i1].m_quad.___u0.v;
                const float *p2 = vertices.m_data[i2].m_quad.___u0.v;
                float e1[3], e2[3], n[3], lenSq;
                int c;

                vertexBase += faceVertCount;

                for (c = 0; c < 3; ++c)
                {
                    e1[c] = p1[c] - p0[c];
                    e2[c] = p2[c] - p0[c];
                }
                n[0] = e1[1] * e2[2] - e1[2] * e2[1];
                n[1] = e1[2] * e2[0] - e1[0] * e2[2];
                n[2] = e1[0] * e2[1] - e1[1] * e2[0];
                lenSq = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];

                if (lenSq == 0.0f)
                {
                    /* degenerate face: append the pool's leading plane as filler */
                    self->m_planeEquations.m_data[self->m_planeEquations.m_size++] = savedPlanes[0];
                }
                else
                {
                    float invLen = 1.0f / sqrtf(lenSq);
                    int best = 0;
                    float bestDot;
                    int j, last;

                    n[0] *= invLen; n[1] *= invLen; n[2] *= invLen;

                    bestDot = savedPlanes[0].m_quad.___u0.v[0] * n[0]
                            + savedPlanes[0].m_quad.___u0.v[1] * n[1]
                            + savedPlanes[0].m_quad.___u0.v[2] * n[2];
                    for (j = 1; j < poolCount; ++j)
                    {
                        float d = savedPlanes[j].m_quad.___u0.v[0] * n[0]
                                + savedPlanes[j].m_quad.___u0.v[1] * n[1]
                                + savedPlanes[j].m_quad.___u0.v[2] * n[2];
                        if (d > bestDot) { bestDot = d; best = j; }
                    }

                    /* append best-matching plane, swap-remove it from the pool */
                    self->m_planeEquations.m_data[self->m_planeEquations.m_size++] = savedPlanes[best];
                    --poolCount;
                    last = poolCount;
                    savedPlanes[best] = savedPlanes[last];
                }
            }
            if (++faceIndex >= conn->m_numVerticesPerFace.m_size)
                break;
        }
    }

    /* append any planes still in the pool */
    if (poolCount > 0)
        hkArray_hkVector4_spliceInto(&self->m_planeEquations, self->m_planeEquations.m_size, 0,
                                     savedPlanes, poolCount);

    hkThreadMemory_deallocateStack(mem, vertices.m_data);
    if (vertices.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, vertices.m_data, 16 * vertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (savedCapMarker >= 0)
        hkThreadMemory_deallocateChunk(mem, savedPlanes, 16 * savedCapMarker, HK_MEMORY_CLASS_ARRAY);
}
