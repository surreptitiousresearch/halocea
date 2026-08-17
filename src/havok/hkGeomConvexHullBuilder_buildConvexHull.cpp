/* ?buildConvexHull@hkGeomConvexHullBuilder@@SAXABUhkGeomConvexHullTolerances@@PBVhkVector4@@HAAVhkGeomHull@@AAV?$hkArray@VhkVector4@@@@@Z @0x83FDE968 */
#include "../headers/havok/hkGeomConvexHullBuilder_internal.h"

/* hkArrayUtil::_reserveMore — SDK-floor boundary. */
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);

/* hkGeomConvexHullBuilder::buildConvexHull
   Top-level hull driver. Copies the input points into usedVerticesOut, optionally
   normalises them to a unit cube, x-sorts and welds coincident points, drops
   collinear ones, then repeatedly builds a sub-hull and post-filters vertices until
   the post-filter reports no further change, finally undoing the unit-cube mapping.

   DEVIATION: the decompiler mistyped the tolerances pointer as hkArray<hkVector4>*;
   it is hkGeomConvexHullTolerances* (m_runConvertToUnitCube at byte 1,
   m_accurateButSlow at byte 2, m_weld_tolerance at offset 4). The
   convert-to/from-unit-cube scratch (a 2-wide scale slot and an offset slot) is
   modelled as two hkVector4 locals. */
void hkGeomConvexHullBuilder_buildConvexHull(const hkGeomConvexHullTolerances *tolerances,
                                             const hkVector4 *verts, int numVertices,
                                             hkGeomHull *hullOut, hkArray<hkVector4> *usedVerticesOut)
{
    hkVector4 unitOffset;
    hkVector4 unitScale;
    int numUnique;
    char changed;      /* m_bool */
    int loopedOnce;    /* v18 */
    hkBool filtered;   /* v24 */
    int i;

    usedVerticesOut->m_size = 0;
    for (i = 0; i < numVertices; ++i)
    {
        int n;
        if (usedVerticesOut->m_size == (usedVerticesOut->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&usedVerticesOut->m_data, 16);
        n = usedVerticesOut->m_size;
        usedVerticesOut->m_data[n] = verts[i];
        usedVerticesOut->m_size = n + 1;
    }

    unitOffset.m_quad.___u0.v[0] = 0.0f;
    unitOffset.m_quad.___u0.v[1] = 0.0f;
    unitOffset.m_quad.___u0.v[2] = 0.0f;
    unitOffset.m_quad.___u0.v[3] = 0.0f;

    if (tolerances->m_runConvertToUnitCube.m_bool)
        hkGeomConvexHullBuilder_convertToUnitCube(usedVerticesOut, &unitScale, &unitOffset);

    if (usedVerticesOut->m_size > 1)
        hkAlgorithm_quickSortRecursive_hkVector4(usedVerticesOut->m_data, 0, usedVerticesOut->m_size - 1,
                                                 hkGeomConvexHullBuilder_vectorLessAndMergeCoordinates);

    hkGeomConvexHullBuilder_weldXsortedVertices(tolerances->m_weld_tolerance, usedVerticesOut, &numUnique);

    if (tolerances->m_accurateButSlow.m_bool && numUnique < 300)
        hkGeomConvexHullBuilder_removeCollinearVertices(usedVerticesOut, 0.001f);

    changed = 1;
    loopedOnce = 0;
    filtered.m_bool = 1;
    while (!loopedOnce || changed)
    {
        loopedOnce = (changed == 0);
        for (i = 0; i < usedVerticesOut->m_size; ++i)
            usedVerticesOut->m_data[i].m_quad.___u0.v[3] = 0.0f;

        hullOut->m_edges.m_size = 0;
        hkGeomConvexHullBuilder_buildConvexSubHull(tolerances, usedVerticesOut, 0,
                                                   usedVerticesOut->m_size - 1, hullOut);
        hkGeomConvexHullBuilder_removeUnusedVertices(hullOut, usedVerticesOut);
        hkGeomConvexHullBuilder_removeFlaggedVertices(usedVerticesOut);
        hkGeomConvexHullBuilder_postFilterVertices(hullOut, 0, usedVerticesOut->m_size - 1,
                                                   tolerances, &filtered);
        hkGeomConvexHullBuilder_removeFlaggedVertices(usedVerticesOut);
        changed = filtered.m_bool;
    }

    if (tolerances->m_runConvertToUnitCube.m_bool)
        hkGeomConvexHullBuilder_convertFromUnitCube(usedVerticesOut, &unitScale, &unitOffset);
}
