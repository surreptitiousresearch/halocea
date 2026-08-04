#pragma once
#include "hkArray.h"
#include "hkGeometryMatchingUtils_TriangleMap.h" /* TriangleMap */

typedef struct hkVector4    hkVector4;
typedef struct hkdGeometry  hkdGeometry;

/* hkdAssetProcessingUtil::SplitPlaneSources (size 72).
   Parallel arrays (one entry per split geometry) holding, for every split piece,
   its reference geometry and the lazily built triangle map. */
typedef struct hkdAssetProcessingUtil_SplitPlaneSources
{
    hkArray<hkVector4 const *> m_referenceVertices;        /* 0  */
    hkArray<int const *>       m_referenceTriangleIndices; /* 12 */
    hkArray<hkArray<int> *>    m_numVerticesPerSection;    /* 24 */
    hkArray<hkArray<int> *>    m_numTrianglesPerSection;   /* 36 */
    hkArray<hkdGeometry *>     m_geometries;               /* 48 */
    hkArray<TriangleMap *>     m_triangleMaps;             /* 60 — DB hkArray<hkGeometryMatchingUtils::TriangleMap *> */
} hkdAssetProcessingUtil_SplitPlaneSources;
