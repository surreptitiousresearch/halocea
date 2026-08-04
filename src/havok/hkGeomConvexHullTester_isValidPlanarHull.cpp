#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkGeomConvexHullTester::isValidPlanarHull
   Validates a degenerate (planar) hull: first checks that every used vertex satisfies the
   plane equations (checkPlaneEquations), then for each tangent plane confirms that it shares
   its three edges consistently with the other tangent planes (findSameEdges accumulates three
   "found" flags across all other planes; all three must end set). hull is unused for the
   planar case but retained to match the ABI. */
hkBool *hkGeomConvexHullTester_isValidPlanarHull(hkBool *result, hkArray<hkVector4> *initialVerts,
        hkGeomHull *hull, hkArray<hkVector4> *usedVerts, hkArray<hkVector4> *planeEquations,
        hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *tangentPlanes, float coplanarTolerance)
{
    hkBool checkResult, sameEdges;
    int n, i, j;

    (void)hull;

    result->m_bool = 1;
    hkGeomConvexHullTester_checkPlaneEquations(&checkResult, initialVerts, usedVerts,
                                               planeEquations, coplanarTolerance);
    result->m_bool = (char)(checkResult.m_bool != 0);

    n = tangentPlanes->m_size;
    for (i = 0; i < n; ++i)
    {
        hkBool foundA, foundB, foundC;
        foundA.m_bool = 0;
        foundB.m_bool = 0;
        foundC.m_bool = 0;

        for (j = 0; j < n; ++j)
        {
            if (j != i)
                hkGeomConvexHullTester_findSameEdges(&sameEdges, &tangentPlanes->m_data[i],
                                                     &tangentPlanes->m_data[j],
                                                     &foundA, &foundB, &foundC);
        }

        result->m_bool = (char)(result->m_bool && foundA.m_bool && foundB.m_bool && foundC.m_bool);
    }

    return result;
}
