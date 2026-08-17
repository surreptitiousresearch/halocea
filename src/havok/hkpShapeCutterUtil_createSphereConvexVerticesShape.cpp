/* ?createSphereConvexVerticesShape@hkpShapeCutterUtil@@SAPAVhkpConvexVerticesShape@@MH@Z @0x83F951B8 */
#include <math.h>
#include "../headers/havok/hkpShapeCutterUtil_boundary.h"

/* hkpShapeCutterUtil::createSphereConvexVerticesShape
   Build a UV-sphere convex hull of the given `radius`, tessellated with
   `numSegments` latitude/longitude divisions. Returns a new
   hkpConvexVerticesShape owning its connectivity.

   DEVIATION: a prior reconstruction carried a phantom 3rd parameter; the
   binary (?createSphereConvexVerticesShape@hkpShapeCutterUtil@@SAPAV...@MH@Z,
   0x83F951B8) takes exactly (f1 radius, r4 numRowsIn) and r4 drives the
   tessellation; spelled `numSegments` below.

   DEVIATION: the vertex writes are VMX128 stvx128 stores; reconstructed as
   scalar float-lane assignments. */
hkpConvexVerticesShape *hkpShapeCutterUtil_createSphereConvexVerticesShape(float radius, int numRowsIn)
{
    hkArray<hkVector4> vertices;
    hkArray<hkVector4> planeEquations;
    hkpConvexVerticesConnectivity *connectivity;
    hkVector4 vtx;
    float latitudeStep, longitudeStep;
    int numRows, numCols;
    int row, col;

    unsigned int numSegments = (unsigned int)numRowsIn;

    vertices.m_data = nullptr;       vertices.m_size = 0;       vertices.m_capacityAndFlags = 0x80000000;
    planeEquations.m_data = nullptr; planeEquations.m_size = 0; planeEquations.m_capacityAndFlags = 0x80000000;

    numRows = (int)numSegments + 1;
    numCols = numRows - 1;                       /* == numSegments */
    longitudeStep = 6.2831855f / (float)numSegments;
    latitudeStep  = 3.1415927f / (float)numSegments;

    /* ---- vertices: one ring per row; poles collapse to a single vertex ---- */
    for (row = 0; row < numRows; ++row)
    {
        float theta = (float)row * latitudeStep;
        float ringRadius = sinf(theta) * radius;
        float z = cosf(theta) * radius;

        for (col = 0; col < numRows; ++col)
        {
            float phi = (float)col * longitudeStep;
            vtx.m_quad.___u0.v[0] = cosf(phi) * ringRadius;
            vtx.m_quad.___u0.v[1] = sinf(phi) * ringRadius;
            vtx.m_quad.___u0.v[2] = z;
            vtx.m_quad.___u0.v[3] = 0.0f;
            hkpShapeCutterUtil_pushVertex(&vertices, &vtx);

            if (row == 0 || row == numRows - 1)
                break;                            /* pole: single vertex */
        }
    }

    /* ---- connectivity ---- */
    connectivity = hkpShapeCutterUtil_newConnectivity();
    {
        int lastVertex = vertices.m_size - 1;         /* south pole */
        int base = ((int)numSegments - 2) * numRows;
        int capBase = base + 1;
        int t;

        /* top + bottom triangle fans */
        if (numCols > 0)
        {
            for (t = 2; t - 2 < numCols; ++t)
            {
                int tri[3];
                tri[0] = t - 1; tri[1] = 0; tri[2] = t;
                hkpShapeCutterUtil__addTriangle(tri, connectivity, &vertices, &planeEquations);
                tri[0] = lastVertex; tri[1] = (base - 1) + t; tri[2] = t + base;
                hkpShapeCutterUtil__addTriangle(tri, connectivity, &vertices, &planeEquations);
            }
        }
        {
            int tri[3];
            tri[0] = numRows; tri[1] = 0; tri[2] = 1;
            hkpShapeCutterUtil__addTriangle(tri, connectivity, &vertices, &planeEquations);
            tri[0] = lastVertex; tri[1] = capBase + numRows - 1; tri[2] = capBase;
            hkpShapeCutterUtil__addTriangle(tri, connectivity, &vertices, &planeEquations);
        }

        /* middle bands of quads */
        if ((int)numSegments - 1 > 1)
        {
            int band = (int)numSegments - 2;
            int ringStart = 1;
            do
            {
                int rowNext = ringStart + numRows;
                int i;
                for (i = 0; i < numCols; ++i)
                {
                    int q[4];
                    q[0] = i + rowNext;
                    q[1] = ringStart + i;
                    q[2] = numRows + 1 + ringStart + i;
                    q[3] = ringStart + 1 + i;
                    hkpShapeCutterUtil__addQuad(q, connectivity, &vertices, &planeEquations);
                }
                {
                    int q[4];
                    q[0] = numCols + rowNext;
                    q[1] = ringStart + numCols;
                    q[2] = ringStart + numRows;
                    q[3] = ringStart;
                    hkpShapeCutterUtil__addQuad(q, connectivity, &vertices, &planeEquations);
                }
                ringStart += numRows;
            } while (--band);
        }
    }

    /* ---- finish: build shape, attach connectivity, free working arrays ---- */
    {
        hkpConvexVerticesShape *shape = hkpShapeCutterUtil_finishShape(&vertices, &planeEquations, connectivity);
        if ((int)vertices.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), vertices.m_data,
                                           16 * vertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
        return shape;
    }
}
