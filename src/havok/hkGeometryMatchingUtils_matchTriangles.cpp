/* ?matchTriangles@hkGeometryMatchingUtils@@SAXABV?$hkArray@UGeometry@hkGeometryMatchingUtils@@@@0MAAUTriangleMap@1@@Z @0x839EB040 */
#include <math.h>
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkPadSpu.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hk1AxisSweep.h"
#include "../headers/havok/hkGeometryMatchingUtils_Geometry.h"
#include "../headers/havok/hkGeometryMatchingUtils_TriangleMap.h"
#include "../headers/havok/hkGeometryMatchingUtils_matchTriangles_scratch.h"
#include "../headers/havok/hkThreadMemory.h"

/* Boundary callees (2-level descent). hkArrayUtil::_reserve grows a raw hkArray.
   The two-array hk1AxisSweep::collide overload (source: hk1AxisSweep_collide.c) takes two aabb sets and appends the
   overlapping (refKey,searchKey) pairs; DEVIATION: distinct C name from the unary
   hk1AxisSweep_collide declared in hk1AxisSweep.h (same C++ symbol, overloaded). */
extern void hkArrayUtil__reserve(void *array, int numElems, int sizeElem);            /* hkArrayUtil::_reserve */
extern int hk1AxisSweep_collidePair(const hk1AxisSweep_AabbInt *aabbsA, int numA,
                                         const hk1AxisSweep_AabbInt *aabbsB, int numB,
                                         hkKeyPair *pairsOut, int pairsCapacity,
                                         int *numPairsSkipped);                     /* hk1AxisSweep::collide */

/* Euclidean distance between two positions using the first three lanes. */
static float vertexDistance(const hkVector4 *a, const hkVector4 *b)
{
    float dx = a->m_quad.___u0.v[0] - b->m_quad.___u0.v[0];
    float dy = a->m_quad.___u0.v[1] - b->m_quad.___u0.v[1];
    float dz = a->m_quad.___u0.v[2] - b->m_quad.___u0.v[2];
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

/* Mean of the three corresponding-vertex distances for one candidate pairing.
   DEVIATION: the original computes this with VMX128 (vmsum3fp128 + rsqrt/Newton
   refine, then a 1/3 scale); reproduced here as the equivalent scalar mean. */
static float meanVertexDistance(const hkVector4 *ra, const hkVector4 *rb, const hkVector4 *rc,
                                const hkVector4 *sa, const hkVector4 *sb, const hkVector4 *sc)
{
    return (vertexDistance(ra, sa) + vertexDistance(rb, sb) + vertexDistance(rc, sc)) * (1.0f / 3.0f);
}

/* Record a candidate match for search triangle searchKey when it beats the best
   distance seen so far. */
static void considerMatch(float *bestDistance, hkGeometryMatchingUtils_TriangleMap_Hit *hit,
                          float candidate, unsigned short referenceGeometry,
                          unsigned char indexOffset, int referenceTriangle, unsigned char flipped)
{
    if (candidate < *bestDistance)
    {
        *bestDistance = candidate;
        hit->m_geometryIndex = referenceGeometry;
        hit->m_indexOffset = indexOffset;
        hit->m_triangleIndex = referenceTriangle;
        hit->m_flippedWinding = flipped;
    }
}

/* Fill an expanded triangle AABB (bounds of its 3 vertices, grown by maxDistance
   on every side) for the broadphase. */
static void expandedTriangleAabb(hkAabb *out, const hkVector4 *vtx0, const hkVector4 *vtx1,
                                 const hkVector4 *vtx2, float maxDistance)
{
    int lane;
    for (lane = 0; lane < 3; ++lane)
    {
        float a = vtx0->m_quad.___u0.v[lane];
        float b = vtx1->m_quad.___u0.v[lane];
        float c = vtx2->m_quad.___u0.v[lane];
        float lo = a < b ? a : b; lo = lo < c ? lo : c;
        float hi = a > b ? a : b; hi = hi > c ? hi : c;
        out->m_min.m_quad.___u0.v[lane] = lo - maxDistance;
        out->m_max.m_quad.___u0.v[lane] = hi + maxDistance;
    }
    out->m_min.m_quad.___u0.v[3] = 0.0f;
    out->m_max.m_quad.___u0.v[3] = 0.0f;
}

/* Flatten one triangle-soup array into per-triangle broadphase AABBs and a
   leaf->(geometry,localTriangle) table. Returns the number of triangles emitted;
   pads the AABB array with (count+4) end-of-list sentinels. startIndexPerGeometry,
   when non-null, receives the first flattened index of each geometry. */
static int flattenTriangles(const hkArray<hkGeometryMatchingUtils_Geometry> *soup,
                            float maxDistance, hk1AxisSweep_AabbInt *aabbs,
                            hkMatchTriangleLeaf *leaves, unsigned int *startIndexPerGeometry,
                            int totalTriangles)
{
    int global = 0;
    int g;
    for (g = 0; g < soup->m_size; ++g)
    {
        const hkGeometryMatchingUtils_Geometry *geom = &soup->m_data[g];
        int t;
        if (startIndexPerGeometry)
            startIndexPerGeometry[g] = global;
        for (t = 0; t < geom->m_numTriangles; ++t)
        {
            const int *tri = &geom->m_triangleIndices[3 * t];
            hkAabb aabb;
            expandedTriangleAabb(&aabb, &geom->m_vertices[tri[0]], &geom->m_vertices[tri[1]],
                                 &geom->m_vertices[tri[2]], maxDistance);
            hk1AxisSweep_AabbInt_set(&aabbs[global], &aabb, global);
            leaves[global].m_geometryIndex = (unsigned short)g;
            leaves[global].m_localTriangleIndex = t;
            ++global;
        }
    }
    (void)totalTriangles;
    return global;
}

/* hkGeometryMatchingUtils::matchTriangles — for every triangle in searchTriangles,
   find the closest triangle in referenceTriangles (within maxDistance, comparing
   all three vertex rotations and both windings) and record the best match in
   triangleMapOut. Triangles of both soups are flattened into a 1-axis sweep
   broadphase; candidate pairs are then scored by mean corresponding-vertex
   distance. */
void hkGeometryMatchingUtils_matchTriangles(
    const hkArray<hkGeometryMatchingUtils_Geometry> *referenceTriangles,
    const hkArray<hkGeometryMatchingUtils_Geometry> *searchTriangles,
    float maxDistance,
    hkGeometryMatchingUtils_TriangleMap *triangleMapOut)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int numRefTris = 0;
    int numSearchTris = 0;
    int g;
    int i;

    hk1AxisSweep_AabbInt *refAabbs;
    hkMatchTriangleLeaf *refLeaves;
    hk1AxisSweep_AabbInt *searchAabbs;
    hkMatchTriangleLeaf *searchLeaves;
    hkKeyPair *pairs;
    float *bestDistance;
    int numPairs;
    int pairCapacity;
    hkPadSpu<int> remaining;

    for (g = 0; g < referenceTriangles->m_size; ++g)
        numRefTris += referenceTriangles->m_data[g].m_numTriangles;
    for (g = 0; g < searchTriangles->m_size; ++g)
        numSearchTris += searchTriangles->m_data[g].m_numTriangles;

    /* Reference broadphase. */
    refAabbs = (hk1AxisSweep_AabbInt *)hkThreadMemory_allocateStack(mem, (32 * numRefTris + 144) & ~15);
    refLeaves = (hkMatchTriangleLeaf *)hkThreadMemory_allocateStack(mem, (12 * numRefTris + 16) & ~15);
    flattenTriangles(referenceTriangles, maxDistance, refAabbs, refLeaves, 0, numRefTris);
    for (i = numRefTris; i < numRefTris + 4; ++i)
        refAabbs[i].m_min[0] = 0xFFFFFFFF;

    /* Output sizing: one Hit per search triangle, one start index per search geometry. */
    if ((triangleMapOut->m_foundReferenceTriangle.m_capacityAndFlags & 0x3FFFFFFF) < numSearchTris)
    {
        int want = 2 * (triangleMapOut->m_foundReferenceTriangle.m_capacityAndFlags & 0x3FFFFFFF);
        if (numSearchTris >= want)
            want = numSearchTris;
        hkArrayUtil__reserve(&triangleMapOut->m_foundReferenceTriangle.m_data, want, 12);
    }
    triangleMapOut->m_foundReferenceTriangle.m_size = numSearchTris;
    if ((triangleMapOut->m_startIndexPerGeometry.m_capacityAndFlags & 0x3FFFFFFF) < searchTriangles->m_size)
    {
        int want = 2 * (triangleMapOut->m_startIndexPerGeometry.m_capacityAndFlags & 0x3FFFFFFF);
        if (searchTriangles->m_size >= want)
            want = searchTriangles->m_size;
        hkArrayUtil__reserve(&triangleMapOut->m_startIndexPerGeometry.m_data, want, 4);
    }
    triangleMapOut->m_startIndexPerGeometry.m_size = searchTriangles->m_size;

    /* Search broadphase. */
    searchAabbs = (hk1AxisSweep_AabbInt *)hkThreadMemory_allocateStack(mem, (32 * numSearchTris + 144) & ~15);
    searchLeaves = (hkMatchTriangleLeaf *)hkThreadMemory_allocateStack(mem, (12 * numSearchTris + 16) & ~15);
    flattenTriangles(searchTriangles, maxDistance, searchAabbs, searchLeaves,
                     triangleMapOut->m_startIndexPerGeometry.m_data, numSearchTris);
    for (i = numSearchTris; i < numSearchTris + 4; ++i)
        searchAabbs[i].m_min[0] = 0xFFFFFFFF;

    hk1AxisSweep_sortAabbs(refAabbs, numRefTris);
    hk1AxisSweep_sortAabbs(searchAabbs, numSearchTris);

    /* Gather candidate overlapping pairs, growing the buffer until the sweep
       reports it consumed all remaining work. */
    pairCapacity = 20 * (numRefTris > numSearchTris ? numRefTris : numSearchTris);
    numPairs = 0;
    for (;; pairCapacity += remaining.m_storage)
    {
        pairs = (hkKeyPair *)hkThreadMemory_allocateStack(mem, (8 * (pairCapacity + 2)) & ~15);
        remaining.m_storage = 0;
        /* last param is int* (skipped-pair count); pass the underlying int of the pad. */
        numPairs = hk1AxisSweep_collidePair(refAabbs, numRefTris, searchAabbs, numSearchTris,
                                                 pairs, pairCapacity, &remaining.m_storage);
        if (!remaining.m_storage)
            break;
        hkThreadMemory_deallocateStack(mem, pairs);
    }

    /* Per search triangle best-match state. */
    bestDistance = (float *)hkThreadMemory_allocateStack(mem, (4 * (numSearchTris + 4)) & ~15);
    for (i = 0; i < numSearchTris; ++i)
    {
        hkGeometryMatchingUtils_TriangleMap_Hit *hit = &triangleMapOut->m_foundReferenceTriangle.m_data[i];
        bestDistance[i] = maxDistance;
        hit->m_geometryIndex = 0xFFFF;
        hit->m_indexOffset = 0;
        hit->m_triangleIndex = -1;
    }

    /* Score each candidate pair. */
    for (i = 0; i < numPairs; ++i)
    {
        unsigned int refKey = pairs[i].m_keyA;
        unsigned int searchKey = pairs[i].m_keyB;
        const hkMatchTriangleLeaf *refLeaf = &refLeaves[refKey];
        const hkMatchTriangleLeaf *searchLeaf = &searchLeaves[searchKey];
        const hkGeometryMatchingUtils_Geometry *refGeom = &referenceTriangles->m_data[refLeaf->m_geometryIndex];
        const hkGeometryMatchingUtils_Geometry *searchGeom = &searchTriangles->m_data[searchLeaf->m_geometryIndex];
        const int *refTri = &refGeom->m_triangleIndices[3 * refLeaf->m_localTriangleIndex];
        const int *searchTri = &searchGeom->m_triangleIndices[3 * searchLeaf->m_localTriangleIndex];

        const hkVector4 *ra0 = &refGeom->m_vertices[refTri[0]];
        const hkVector4 *ra1 = &refGeom->m_vertices[refTri[1]];
        const hkVector4 *ra2 = &refGeom->m_vertices[refTri[2]];
        const hkVector4 *sb0 = &searchGeom->m_vertices[searchTri[0]];
        const hkVector4 *sb1 = &searchGeom->m_vertices[searchTri[1]];
        const hkVector4 *sb2 = &searchGeom->m_vertices[searchTri[2]];

        float *best = &bestDistance[searchKey];
        hkGeometryMatchingUtils_TriangleMap_Hit *hit = &triangleMapOut->m_foundReferenceTriangle.m_data[searchKey];
        unsigned short refGeomIndex = refLeaf->m_geometryIndex;
        int refTriIndex = refLeaf->m_localTriangleIndex;

        /* Three vertex rotations x two windings, evaluated (and greedily kept) in
           the same order as the original. */
        considerMatch(best, hit, meanVertexDistance(ra0, ra1, ra2, sb0, sb1, sb2), refGeomIndex, 0, refTriIndex, 0);
        considerMatch(best, hit, meanVertexDistance(ra0, ra1, ra2, sb1, sb2, sb0), refGeomIndex, 2, refTriIndex, 0);
        considerMatch(best, hit, meanVertexDistance(ra0, ra1, ra2, sb2, sb0, sb1), refGeomIndex, 1, refTriIndex, 0);
        considerMatch(best, hit, meanVertexDistance(ra0, ra1, ra2, sb0, sb2, sb1), refGeomIndex, 0, refTriIndex, 1);
        considerMatch(best, hit, meanVertexDistance(ra0, ra1, ra2, sb2, sb0, sb1), refGeomIndex, 1, refTriIndex, 1);
        considerMatch(best, hit, meanVertexDistance(ra0, ra1, ra2, sb2, sb1, sb0), refGeomIndex, 2, refTriIndex, 1);
    }

    /* Release the stack scratch in reverse order. */
    hkThreadMemory_deallocateStack(mem, bestDistance);
    hkThreadMemory_deallocateStack(mem, pairs);
    hkThreadMemory_deallocateStack(mem, searchLeaves);
    hkThreadMemory_deallocateStack(mem, searchAabbs);
    hkThreadMemory_deallocateStack(mem, refLeaves);
    hkThreadMemory_deallocateStack(mem, refAabbs);
}
