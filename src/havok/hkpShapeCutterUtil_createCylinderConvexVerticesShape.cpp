/* ?createCylinderConvexVerticesShape@hkpShapeCutterUtil@@SAPAVhkpConvexVerticesShape@@MMHABVhkTransform@@@Z @0x83F955F8 */
#include <math.h>
#include "../headers/havok/hkpShapeCutterUtil_boundary.h"

/* Append one vertex index (unsigned short) to a connectivity index array. */
static void pushIndex(hkArray<unsigned short> *arr, unsigned short value)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 2);
    arr->m_data[arr->m_size++] = value;
}

/* Append one per-face vertex count (unsigned char). */
static void pushFaceSize(hkArray<unsigned char> *arr, unsigned char value)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 1);
    arr->m_data[arr->m_size++] = value;
}

/* Push a face plane (nx,ny,nz,d) into the plane-equation array. */
static void pushPlane(hkArray<hkVector4> *arr, const hkVector4 *plane)
{
    if (arr->m_size == (arr->m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&arr->m_data, 16);
    arr->m_data[arr->m_size++] = *plane;
}

/* hkpShapeCutterUtil::createCylinderConvexVerticesShape
   Build an `numSegments`-sided cylinder convex hull of the given radius/height,
   oriented by transform `cylinderTransform`. Two rings of vertices (top at z=height, bottom at
   z=0) are placed and transformed into world space; the side faces are quads and
   the two caps are `numSegments`-gons.

   DEVIATION: the original is heavily VMX128 (vertex placement, the cap-plane
   rotate, and the final two-plane transform are all lvx128/vmsum3fp128 blocks).
   Reconstructed as scalar float math. The side-face planes are emitted by
   _addQuad from the already-transformed vertices, so they are world-space;
   the two cap planes are computed directly in world space here
   (worldN = R*localN, worldD = localD - dot(worldN, t)), which is the exact
   forward plane transform the VMX tail performs.

   `numSegs` and `trans` are accepted for signature compatibility but are not
   referenced by the body (side count is driven by `numSides`). */
hkpConvexVerticesShape *hkpShapeCutterUtil_createCylinderConvexVerticesShape(float radius, float height, int numSegs,
                                                                            const hkTransform *trans, int numSides,
                                                                            const hkTransform *cylinderTransform)
{
    hkArray<hkVector4> vertices;
    hkArray<hkVector4> planeEquations;
    hkpConvexVerticesConnectivity *connectivity;
    float angleStep;
    int j;

    (void)numSegs;
    (void)trans;

    vertices.m_data = nullptr;       vertices.m_size = 0;       vertices.m_capacityAndFlags = 0x80000000;
    planeEquations.m_data = nullptr; planeEquations.m_size = 0; planeEquations.m_capacityAndFlags = 0x80000000;

    if (2 * numSides > 0)
        hkArrayUtil__reserve(&vertices.m_data, 2 * numSides, 16);
    vertices.m_size = 2 * numSides;

    /* ---- vertices: top ring [0,numSides), bottom ring [numSides,2*numSides) ---- */
    angleStep = 6.2831855f / (float)numSides;
    for (j = 0; j < numSides; ++j)
    {
        float angle = (float)j * angleStep;
        float px = sinf(angle) * radius;
        float py = cosf(angle) * radius;
        hkVector4 topVtx, bottomVtx;

        topVtx.m_quad.___u0.v[0] = px; topVtx.m_quad.___u0.v[1] = py;
        topVtx.m_quad.___u0.v[2] = height; topVtx.m_quad.___u0.v[3] = 0.0f;
        bottomVtx.m_quad.___u0.v[0] = px; bottomVtx.m_quad.___u0.v[1] = py;
        bottomVtx.m_quad.___u0.v[2] = 0.0f; bottomVtx.m_quad.___u0.v[3] = 0.0f;

        hkVector4_setTransformedPos(&topVtx, cylinderTransform, &topVtx);
        hkVector4_setTransformedPos(&bottomVtx, cylinderTransform, &bottomVtx);

        vertices.m_data[j] = topVtx;
        vertices.m_data[numSides + j] = bottomVtx;
    }

    /* ---- connectivity ---- */
    connectivity = hkpShapeCutterUtil_newConnectivity();

    /* side quads (planes emitted by _addQuad, already world-space) */
    for (j = 1; j - 1 < numSides; ++j)
    {
        int quad[4];
        quad[0] = j % numSides;
        quad[1] = j % numSides + numSides;
        quad[2] = numSides - 1 + j;
        quad[3] = j - 1;
        hkpShapeCutterUtil__addQuad(quad, connectivity, &vertices, &planeEquations);
    }

    /* top cap: reversed top ring */
    for (j = 0; j < numSides; ++j)
        pushIndex(&connectivity->m_vertexIndices, (unsigned short)(numSides - 1 - j));
    pushFaceSize(&connectivity->m_numVerticesPerFace, (unsigned char)numSides);
    {
        /* local top plane (0,0,1,-height), transformed to world */
        hkVector4 localN, worldN, worldPlane;
        localN.m_quad.___u0.v[0] = 0.0f; localN.m_quad.___u0.v[1] = 0.0f;
        localN.m_quad.___u0.v[2] = 1.0f; localN.m_quad.___u0.v[3] = 0.0f;
        hkVector4_setRotatedDir(&worldN, &cylinderTransform->m_rotation, &localN);
        worldPlane = worldN;
        worldPlane.m_quad.___u0.v[3] = -height
            - (worldN.m_quad.___u0.v[0] * cylinderTransform->m_translation.m_quad.___u0.v[0]
             + worldN.m_quad.___u0.v[1] * cylinderTransform->m_translation.m_quad.___u0.v[1]
             + worldN.m_quad.___u0.v[2] * cylinderTransform->m_translation.m_quad.___u0.v[2]);
        pushPlane(&planeEquations, &worldPlane);
    }

    /* bottom cap: bottom ring in order */
    for (j = 0; j < numSides; ++j)
        pushIndex(&connectivity->m_vertexIndices, (unsigned short)(numSides + j));
    pushFaceSize(&connectivity->m_numVerticesPerFace, (unsigned char)numSides);
    {
        /* local bottom plane (0,0,-1,0), transformed to world */
        hkVector4 localN, worldN, worldPlane;
        localN.m_quad.___u0.v[0] = 0.0f; localN.m_quad.___u0.v[1] = 0.0f;
        localN.m_quad.___u0.v[2] = -1.0f; localN.m_quad.___u0.v[3] = 0.0f;
        hkVector4_setRotatedDir(&worldN, &cylinderTransform->m_rotation, &localN);
        worldPlane = worldN;
        worldPlane.m_quad.___u0.v[3] = 0.0f
            - (worldN.m_quad.___u0.v[0] * cylinderTransform->m_translation.m_quad.___u0.v[0]
             + worldN.m_quad.___u0.v[1] * cylinderTransform->m_translation.m_quad.___u0.v[1]
             + worldN.m_quad.___u0.v[2] * cylinderTransform->m_translation.m_quad.___u0.v[2]);
        pushPlane(&planeEquations, &worldPlane);
    }

    /* ---- finish ---- */
    {
        hkpConvexVerticesShape *shape = hkpShapeCutterUtil_finishShape(&vertices, &planeEquations, connectivity);
        if ((int)vertices.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), vertices.m_data,
                                           16 * vertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
        return shape;
    }
}
