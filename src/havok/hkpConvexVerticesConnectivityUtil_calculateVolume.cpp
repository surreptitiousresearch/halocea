#include "../headers/havok/hkpConvexVerticesShape.h"

/* hkpConvexVerticesConnectivityUtil::calculateVolume @ 0x83F91AA8
   Signed volume of the convex hull described by `vertices` + the per-face vertex
   index lists in `conn`. Each face is triangulated as a fan from its first
   vertex; the per-triangle contribution is the z-projected prism term
   crossZ(p1-p0, p2-p0) * (p0.z + p1.z + p2.z), summed and scaled by 1/6.
   DEVIATION: the VMX cross/dot accumulation is reconstructed as scalar float. */
float hkpConvexVerticesConnectivityUtil_calculateVolume(const hkArray<hkVector4> *vertices,
                                                        const hkpConvexVerticesConnectivity *conn)
{
    int numFaces = conn->m_numVerticesPerFace.m_size;
    const unsigned char *faceSizes = conn->m_numVerticesPerFace.m_data;
    const unsigned short *indices = conn->m_vertexIndices.m_data;
    const hkVector4 *verts = vertices->m_data;
    float acc = 0.0f;
    int vertexCursor = 0;
    int f;

    for (f = 0; f < numFaces; ++f)
    {
        unsigned int k = faceSizes[f];
        if (k)
        {
            const float *p0 = verts[indices[vertexCursor]].m_quad.___u0.vector4_f32;
            unsigned int j;
            for (j = 1; j + 1 < k; ++j)
            {
                const float *p1 = verts[indices[vertexCursor + j]].m_quad.___u0.vector4_f32;
                const float *p2 = verts[indices[vertexCursor + j + 1]].m_quad.___u0.vector4_f32;
                float e1x = p1[0] - p0[0], e1y = p1[1] - p0[1];
                float e2x = p2[0] - p0[0], e2y = p2[1] - p0[1];
                float crossZ = e1x * e2y - e1y * e2x;
                acc += crossZ * (p0[2] + p1[2] + p2[2]);
            }
            vertexCursor += (int)k;
        }
    }

    return acc * (1.0f / 6.0f);
}
