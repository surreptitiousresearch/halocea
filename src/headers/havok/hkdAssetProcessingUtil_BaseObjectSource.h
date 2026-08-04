#pragma once
#include "hkArray.h"
#include "hkGeometryMatchingUtils_TriangleMap.h"

typedef struct hkVector4 hkVector4;
typedef struct hkdBreakableShape hkdBreakableShape;

/* hkdAssetProcessingUtil::BaseObjectSource (size 40).
   Reference geometry for the un-split base breakable shape, plus the lazily
   built triangle map matching its geometry to the reference triangles. */
typedef struct hkdAssetProcessingUtil_BaseObjectSource
{
    const hkVector4 *m_referenceVertices;        /* 0  */
    const int *m_referenceTriangleIndices;       /* 4  */
    hkArray<int> m_numVerticesPerSection;        /* 8  */
    hkArray<int> m_numTrianglesPerSection;       /* 20 */
    hkdBreakableShape *m_breakableShape;         /* 32 */
    TriangleMap *m_triangleMap;                  /* 36 — DB hkGeometryMatchingUtils::TriangleMap * */
} hkdAssetProcessingUtil_BaseObjectSource;
