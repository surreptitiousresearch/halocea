/* ?isValidHull@hkGeomConvexHullTester@@SA?AVhkBool@@ABUhkGeomConvexHullTolerances@@PBVhkVector4@@HAAVhkGeomHull@@AAV?$hkArray@VhkVector4@@@@@Z @0x83FE3E48 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include "../headers/havok/hkThreadMemory.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem);

/* hkGeomConvexHullTester::isValidHull
   Validates that `hull` really is the convex hull of usedVerts. It re-derives the
   hull's plane equations (and tangent/bevel planes) with buildPlaneEquations, then
   dispatches to the planar or non-planar validator depending on whether the plane
   set was found to be degenerate (coplanar). The initial vertices are copied into a
   private array so the (optional) unit-cube normalisation does not disturb the
   caller's data. All three scratch arrays are freed on exit.

   DEVIATION: the raw thread-memory frees `*(hkThreadMemory**)(*(int*)r13+704)` are
   wrapped in hkThreadMemory_getCurrent(); buildPlaneEquations' result slot (a bool)
   was typed hkGeomEdge* by the decompiler. */
hkBool *hkGeomConvexHullTester_isValidHull(hkBool *result, const hkGeomConvexHullTolerances *tolerances,
                                           const hkVector4 *initialVertsPointer, int numInitialVertices,
                                           hkGeomHull *hull, hkArray<hkVector4> *usedVerts)
{
    float coplanarTolerance = tolerances->m_coplanar_tolerance;
    hkBool planeEqResult;                                   /* v20 */
    hkBool isPlanar;                                        /* v19 */
    hkArray<hkVector4> initialVerts;                        /* v21 */
    hkArray<hkVector4> planeEquations;                      /* v22 */
    hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> tangentPlanes; /* v23 */
    hkVector4 usedScale, usedOffset;                        /* v25, v24 */
    hkVector4 initScale, initOffset;                        /* v27, v26 */
    hkVector4 planarPlaneEquation;                          /* v28 */
    hkBool *verdict;                                        /* v17 */
    int i;

    isPlanar.m_bool = 0;
    tangentPlanes.m_data = nullptr;
    tangentPlanes.m_size = 0;
    tangentPlanes.m_capacityAndFlags = (int)0x80000000;
    planeEquations.m_data = nullptr;
    planeEquations.m_size = 0;
    planeEquations.m_capacityAndFlags = (int)0x80000000;
    initialVerts.m_data = nullptr;
    initialVerts.m_size = 0;
    initialVerts.m_capacityAndFlags = (int)0x80000000;

    for (i = 0; i < numInitialVertices; ++i)
    {
        int n;
        if (initialVerts.m_size == (initialVerts.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&initialVerts.m_data, 16);
        n = initialVerts.m_size;
        initialVerts.m_data[n] = initialVertsPointer[i];
        initialVerts.m_size = n + 1;
    }

    if (tolerances->m_runConvertToUnitCube.m_bool)
    {
        hkGeomConvexHullBuilder_convertToUnitCube(usedVerts, &usedScale, &usedOffset);
        hkGeomConvexHullBuilder_convertToUnitCube(&initialVerts, &initScale, &initOffset);
    }

    result->m_bool = 1;
    hkGeomConvexHullBuilder_buildPlaneEquations(&planeEqResult, tolerances, hull, usedVerts,
                                                &planarPlaneEquation /* v28 */,
                                                &isPlanar, &planeEquations, &tangentPlanes);

    if (isPlanar.m_bool)
        verdict = hkGeomConvexHullTester_isValidPlanarHull(&planeEqResult, &initialVerts, hull, usedVerts,
                                                           &planeEquations, &tangentPlanes, coplanarTolerance);
    else
        verdict = hkGeomConvexHullTester_isValidNonPlanarHull(&planeEqResult, &initialVerts, hull, usedVerts,
                                                              &planeEquations, &tangentPlanes, coplanarTolerance);
    result->m_bool = verdict->m_bool;

    if (tolerances->m_runConvertToUnitCube.m_bool)
    {
        hkGeomConvexHullBuilder_convertFromUnitCube(usedVerts, &usedScale, &usedOffset);
        hkGeomConvexHullBuilder_convertFromUnitCube(&initialVerts, &initScale, &initOffset);
    }

    if (initialVerts.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), initialVerts.m_data,
                                       16 * initialVerts.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (planeEquations.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), planeEquations.m_data,
                                       16 * planeEquations.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (tangentPlanes.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), tangentPlanes.m_data,
                                       32 * tangentPlanes.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    return result;
}
