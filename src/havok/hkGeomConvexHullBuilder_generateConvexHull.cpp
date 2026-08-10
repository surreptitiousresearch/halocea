#include "../headers/havok/hkGeomConvexHull.h"
#include "../headers/havok/hkThreadMemory.h"

/* Level-1 boundary callees (bounded descent — kept extern). Signatures verified
   against disasm (0x83FDECAC..0x83FDECE8 etc.). */
extern void hkArrayUtil__reserveMore(void *arrayData, int elementSize);                          /* hkArrayUtil::_reserveMore */
extern hkBool *hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates(hkBool *result, hkVector4 *a, hkVector4 *b); /* comparator */
extern void hkAlgorithm_quickSortRecursive_hkVector4(hkVector4 *data, int lo, int hi,
        hkBool *(*cmp)(hkBool *, hkVector4 *, hkVector4 *));                                     /* hkAlgorithm::quickSortRecursive<hkVector4,...> */
extern void hkGeomConvexHullBuilder_weldXsortedVertices(float weldTolerance, hkArray<hkVector4> *verts, int *numVerticesInOut); /* hkGeomConvexHullBuilder::weldXsortedVertices */
extern void hkGeomConvexHullBuilder_buildConvexHull(const hkGeomConvexHullTolerances *tol, const hkVector4 *verts,
        int numVertices, hkGeomHull *hullOut, hkArray<hkVector4> *usedVerticesOut);              /* hkGeomConvexHullBuilder::buildConvexHull */
extern hkBool *hkGeomConvexHullTester_isValidHull(hkBool *result, const hkGeomConvexHullTolerances *tol,
        const hkVector4 *verts, int numVertices, hkGeomHull *hull, hkArray<hkVector4> *usedVertices); /* hkGeomConvexHullTester::isValidHull */
extern hkBool *hkGeomConvexHullBuilder_buildPlaneEquations(hkBool *result, const hkGeomConvexHullTolerances *tol,
        hkGeomHull *hull, hkArray<hkVector4> *usedVertices, hkVector4 *centroidOut, hkBool *okOut,
        hkArray<hkVector4> *planesOut, hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *planeAndPointsOut); /* hkGeomConvexHullBuilder::buildPlaneEquations */
extern void hkGeomConvexHullBuilder_removeCollinearVertices(hkArray<hkVector4> *verts, float tolerance);    /* hkGeomConvexHullBuilder::removeCollinearVertices */
extern void hkGeomConvexHullBuilder_generateHullFromPlanarPoints(const hkVector4 *centroid, const hkVector4 *verts,
        int numVertices, hkArray<hkVector4> *usedVerticesOut, hkArray<hkVector4> *scratchOut);   /* hkGeomConvexHullBuilder::generateHullFromPlanarPoints */

/* hkGeomConvexHullBuilder::generateConvexHull — top-level convex-hull generator.
   It x-sorts a private copy of the input vertices, welds near-duplicates, and
   builds the hull with a fixed tolerance profile. If the fast build fails validity
   and accurateButSlow is requested, it retries via the planar-points/plane-equation
   path; otherwise it re-runs buildConvexHull with progressively looser degeneracy
   tolerances (post-filter on, degenerate 4e-6 -> 2e-7, _tol3 2e-5 -> 1e-6).

   The tolerance constants and control flow are transcribed from the decompile.
   DEVIATION: original stores the private/scratch arrays in inline stack storage
   (64 elements each, DONT_DEALLOCATE) and open-codes the copy/growth as VMX128;
   reconstructed with explicit inline storage buffers and scalar element copies.
   All hull-builder callees stay extern (bounded descent). */
void hkGeomConvexHullBuilder_generateConvexHull(const hkVector4 *verts, int numVertices,
                                                hkGeomHull *hullOut, hkArray<hkVector4> *usedVerticesOut,
                                                hkGeomConvexHullMode mode)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    hkGeomConvexHullTolerances tol;
    hkVector4 sortedStorage[64];
    hkVector4 scratchStorage[64];
    hkVector4 planarStorage[64];
    hkGeomConvexHullBuilder_PlaneAndPoints planeAndPointsStorage[64];
    hkArray<hkVector4> sortedVerts = { sortedStorage, 0, (int)0x80000040 };  /* cap 64 | DONT_DEALLOCATE */
    hkArray<hkVector4> scratchVerts = { scratchStorage, 0, (int)0x80000040 };
    hkArray<hkVector4> planarVerts = { planarStorage, 0, (int)0x80000040 };
    hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> planeAndPoints = { planeAndPointsStorage, 0, (int)0x80000040 };
    hkVector4 centroid;
    hkBool valid, planeOk, scratchBool[16];
    int weldCount;
    int i;

    tol.m_weld_tolerance              = 0.000019999999f;
    tol.m_coplanar_vertices_tolerance = 0.000001f;
    tol.m_oppositeNormal_tolerance    = 0.000001f;
    tol.m__min_proj                   = 0.000001f;
    tol.m__planeEqnMinLength          = 0.000001f;
    tol.m_coplanar_plane_tolerance    = 0.0000099999997f;
    tol.m_coplanar_tolerance          = 0.050000001f;
    tol.m__maxAngle                   = 0.0000000099999999f;
    tol.m__tol                        = 0.000099999997f;
    tol.m__tol2                       = 0.0000099999997f;
    tol.m__tol3                       = 0.000019999999f;
    tol.m_postFilter.m_bool           = 0;
    tol.m_degenerate_tolerance        = 0.000004f;
    tol.m_runConvertToUnitCube.m_bool = 0;
    tol.m_accurateButSlow.m_bool      = (mode == HK_GEOM_CONVEXHULL_MODE_ACCURATE_BUT_SLOW) ? 1 : 0;

    /* private x-sorted copy of the input */
    for (i = 0; i < numVertices; ++i)
    {
        if (sortedVerts.m_size == (sortedVerts.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&sortedVerts.m_data, 16);
        sortedVerts.m_data[sortedVerts.m_size++] = verts[i];
    }
    if (sortedVerts.m_size > 1)
        hkAlgorithm_quickSortRecursive_hkVector4(sortedVerts.m_data, 0, sortedVerts.m_size - 1,
                                                 hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates);

    weldCount = sortedVerts.m_size;
    hkGeomConvexHullBuilder_weldXsortedVertices(tol.m_weld_tolerance, &sortedVerts, &weldCount);

    usedVerticesOut->m_size = 0;
    hkGeomConvexHullBuilder_buildConvexHull(&tol, sortedVerts.m_data, sortedVerts.m_size, hullOut, usedVerticesOut);
    valid.m_bool = hkGeomConvexHullTester_isValidHull(&scratchBool[0], &tol, sortedVerts.m_data,
                                                      sortedVerts.m_size, hullOut, usedVerticesOut)->m_bool;

    if (!valid.m_bool && tol.m_accurateButSlow.m_bool)
    {
        scratchVerts.m_size = 0;
        planeOk.m_bool = 0;
        hkGeomConvexHullBuilder_buildPlaneEquations(&valid, &tol, hullOut, usedVerticesOut, &centroid,
                                                    &planeOk, &scratchVerts, &planeAndPoints);
        if (planeOk.m_bool)
        {
            usedVerticesOut->m_size = 0;
            hkGeomConvexHullBuilder_removeCollinearVertices(&sortedVerts, 0.001f);
            planarVerts.m_size = 0;
            hkGeomConvexHullBuilder_generateHullFromPlanarPoints(&centroid, sortedVerts.m_data,
                                                                 sortedVerts.m_size, usedVerticesOut, &planarVerts);
            valid.m_bool = hkGeomConvexHullTester_isValidHull(&valid, &tol, verts, numVertices,
                                                              hullOut, usedVerticesOut)->m_bool;
            if (planarVerts.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(mem, planarVerts.m_data,
                                               16 * planarVerts.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
        }
        if (planeAndPoints.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(mem, planeAndPoints.m_data,
                                           32 * planeAndPoints.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    }

    if (!tol.m_accurateButSlow.m_bool)
    {
        tol.m_runConvertToUnitCube.m_bool = 1;
        if (!valid.m_bool)
        {
            tol.m_postFilter.m_bool = 1;
            hkGeomConvexHullBuilder_buildConvexHull(&tol, verts, numVertices, hullOut, usedVerticesOut);
            if (!hkGeomConvexHullTester_isValidHull(&valid, &tol, verts, numVertices, hullOut, usedVerticesOut)->m_bool)
            {
                tol.m_degenerate_tolerance = 0.0000002f;
                hkGeomConvexHullBuilder_buildConvexHull(&tol, verts, numVertices, hullOut, usedVerticesOut);
                if (!hkGeomConvexHullTester_isValidHull(&valid, &tol, verts, numVertices, hullOut, usedVerticesOut)->m_bool)
                {
                    tol.m__tol3 = 0.000001f;
                    hkGeomConvexHullBuilder_buildConvexHull(&tol, verts, numVertices, hullOut, usedVerticesOut);
                    hkGeomConvexHullTester_isValidHull(&valid, &tol, verts, numVertices, hullOut, usedVerticesOut);
                }
            }
        }
    }

    if (scratchVerts.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, scratchVerts.m_data, 16 * scratchVerts.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (sortedVerts.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, sortedVerts.m_data, 16 * sortedVerts.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
}
