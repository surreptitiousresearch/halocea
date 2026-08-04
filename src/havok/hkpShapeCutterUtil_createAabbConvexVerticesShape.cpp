#include "../headers/havok/hkpShapeCutterUtil_boundary.h"
#include "../headers/havok/hkAabb.h"

/* hkpShapeCutterUtil::createAabbConvexVerticesShape
   Build a box convex hull from an axis-aligned bounding box: 8 corner vertices,
   6 face planes, and a fixed 6-quad connectivity table.

   DEVIATION: the original stages every corner/plane through VMX128
   lvx128/stvx128 into stack slots; reconstructed here as scalar float writes.
   The 8-corner ordering uses the standard bit convention (bit0=x, bit1=y,
   bit2=z; 0=min, 1=max), consistent with the hardcoded face-index table below;
   the plane equations and index table are read verbatim from the binary. */
hkpConvexVerticesShape *hkpShapeCutterUtil_createAabbConvexVerticesShape(const hkAabb *aabb)
{
    float minX = aabb->m_min.m_quad.___u0.v[0];
    float minY = aabb->m_min.m_quad.___u0.v[1];
    float minZ = aabb->m_min.m_quad.___u0.v[2];
    float maxX = aabb->m_max.m_quad.___u0.v[0];
    float maxY = aabb->m_max.m_quad.___u0.v[1];
    float maxZ = aabb->m_max.m_quad.___u0.v[2];

    hkVector4 corners[8];
    hkVector4 planeBuf[6];
    hkArray<hkVector4> vertices;
    hkArray<hkVector4> planeEquations;
    hkpConvexVerticesConnectivity *connectivity;
    int i;

    /* 8 box corners */
    for (i = 0; i < 8; ++i)
    {
        corners[i].m_quad.___u0.v[0] = (i & 1) ? maxX : minX;
        corners[i].m_quad.___u0.v[1] = (i & 2) ? maxY : minY;
        corners[i].m_quad.___u0.v[2] = (i & 4) ? maxZ : minZ;
        corners[i].m_quad.___u0.v[3] = 0.0f;
    }

    /* 6 outward face planes (nx,ny,nz,d) */
    #define SET_PLANE(idx, nx, ny, nz, d) do {                 \
        planeBuf[idx].m_quad.___u0.v[0] = (nx);                \
        planeBuf[idx].m_quad.___u0.v[1] = (ny);                \
        planeBuf[idx].m_quad.___u0.v[2] = (nz);                \
        planeBuf[idx].m_quad.___u0.v[3] = (d);                 \
    } while (0)
    SET_PLANE(0,  1.0f, 0.0f, 0.0f, -maxX);
    SET_PLANE(1, -1.0f, 0.0f, 0.0f,  minX);
    SET_PLANE(2,  0.0f, 1.0f, 0.0f, -maxY);
    SET_PLANE(3,  0.0f,-1.0f, 0.0f,  minY);
    SET_PLANE(4,  0.0f, 0.0f, 1.0f, -maxZ);
    SET_PLANE(5,  0.0f, 0.0f,-1.0f,  minZ);
    #undef SET_PLANE

    planeEquations.m_data = planeBuf;
    planeEquations.m_size = 6;
    planeEquations.m_capacityAndFlags = 0x80000006; /* inline storage, cap 6, don't deallocate */

    vertices.m_data = corners;
    vertices.m_size = 8;
    vertices.m_capacityAndFlags = 0x80000008; /* inline storage */

    /* connectivity: 6 quads, indices verbatim from the binary */
    connectivity = hkpShapeCutterUtil_newConnectivity();
    {
        static const unsigned short faceIndices[24] = {
            1, 3, 7, 5,   0, 4, 6, 2,   2, 6, 7, 3,
            0, 1, 5, 4,   4, 5, 7, 6,   0, 2, 3, 1
        };
        hkArray<unsigned char> *nvpf = &connectivity->m_numVerticesPerFace;
        int m;

        hkArrayUnsignedShort_spliceInto(&connectivity->m_vertexIndices, 0, 0, faceIndices, 24);

        m = nvpf->m_size;
        if (m < 6)
        {
            unsigned int cap = nvpf->m_capacityAndFlags & 0x3FFFFFFF;
            if (cap < 6)
            {
                int newCap = 2 * (int)cap;
                if (newCap <= 6)
                    newCap = 6;
                hkArrayUtil__reserve(&nvpf->m_data, newCap, 1);
            }
            for (; m < 6; ++m)
                nvpf->m_data[m] = 4;
        }
        nvpf->m_size = 6;
    }

    return hkpShapeCutterUtil_finishShape(&vertices, &planeEquations, connectivity);
}
