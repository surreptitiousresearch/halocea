#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"
#include <math.h>

/* hkGeomConvexHullTester::isValidNonPlanarHull
   Validates a full 3D hull. Steps:
     1. checkPlaneEquations — every used vertex satisfies the plane equations.
     2. Each tangent plane must be a real triangle (its three vertices distinct).
     3. No two tangent planes with identical vertex triples may have (near-)identical plane
        equations (a duplicate face); and every plane must share its edges consistently with
        the others (findSameEdges accumulates three flags, all must end set).
     4. Euler characteristic: with >2 used vertices,
        (#faces) - (#edges)/2 + (#vertices) == 2.
   DEVIATION: the VMX per-lane compare of the two plane equations against a 0.001 tolerance
   (vsubfp / vand abs-mask / vcmpgefp / vcmpequw) is reconstructed as a scalar 4-lane
   |diff| <= 0.001 test; the mask-register plumbing (s_invMaskFromBits / positiveMask) is
   folded into abs() and the all-lanes-within-tolerance reduction. */
hkBool *hkGeomConvexHullTester_isValidNonPlanarHull(hkBool *result, hkArray<hkVector4> *initialVerts,
        hkGeomHull *hull, hkArray<hkVector4> *usedVerts, hkArray<hkVector4> *planeEquations,
        hkArray<hkGeomConvexHullBuilder_PlaneAndPoints> *tangentPlanes, float coplanarTolerance)
{
    const float dupTolerance = 0.001f;
    hkBool checkResult, sameEdges;
    int n, i, j, usedCount;

    result->m_bool = 1;
    hkGeomConvexHullTester_checkPlaneEquations(&checkResult, initialVerts, usedVerts,
                                               planeEquations, coplanarTolerance);
    result->m_bool = (char)(checkResult.m_bool != 0);

    n = tangentPlanes->m_size;
    for (i = 0; i < n; ++i)
    {
        hkGeomConvexHullBuilder_PlaneAndPoints *planeA = &tangentPlanes->m_data[i];
        int va1 = planeA->m_v1->m_vertex;
        hkBool foundA, foundB, foundC;

        /* plane A must be a non-degenerate triangle */
        result->m_bool = (char)(result->m_bool
                                && planeA->m_v0->m_vertex != va1
                                && va1 != planeA->m_v2->m_vertex);

        foundA.m_bool = 0;
        foundB.m_bool = 0;
        foundC.m_bool = 0;

        for (j = 0; j < n; ++j)
        {
            if (j != i)
            {
                hkGeomConvexHullBuilder_PlaneAndPoints *planeB = &tangentPlanes->m_data[j];
                int valid;

                if (!result->m_bool)
                {
                    valid = 0;
                }
                else if (planeA->m_v0->m_vertex != planeB->m_v0->m_vertex
                         || planeA->m_v1->m_vertex != planeB->m_v1->m_vertex
                         || planeA->m_v2->m_vertex != planeB->m_v2->m_vertex)
                {
                    valid = 1; /* different vertex triples — fine */
                }
                else
                {
                    /* identical triples: reject if the plane equations coincide */
                    int lane, duplicate = 1;
                    for (lane = 0; lane < 4; ++lane)
                    {
                        float d = planeA->m_planeEquation.m_quad.___u0.v[lane]
                                - planeB->m_planeEquation.m_quad.___u0.v[lane];
                        if (fabsf(d) > dupTolerance)
                        {
                            duplicate = 0;
                            break;
                        }
                    }
                    valid = duplicate ? 0 : 1;
                }

                if (!valid)
                {
                    result->m_bool = 0;
                }
                else
                {
                    hkGeomConvexHullTester_findSameEdges(&sameEdges, planeA, planeB,
                                                         &foundA, &foundB, &foundC);
                    result->m_bool = (char)(sameEdges.m_bool != 0);
                }
            }
        }

        result->m_bool = (char)(result->m_bool && foundA.m_bool && foundB.m_bool && foundC.m_bool);
    }

    usedCount = usedVerts->m_size;
    if (usedCount > 2)
    {
        result->m_bool = (char)(result->m_bool
                                && (n - hull->m_edges.m_size / 2 + usedCount == 2));
    }

    return result;
}
