/* rematchGraphics @0x8383FCA8 */
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkGeometryMatchingUtils_TriangleMap.h"
#include "../headers/havok/hkdAssetProcessingUtil_BaseObjectSource.h"
#include "../headers/havok/hkdAssetProcessingUtil_SplitPlaneSources.h"

/* Memory-class tag: DB exposes the enumerator name at the call sites but not its
   numeric value, so it is externed rather than invented (cf. hkThreadMemory.h). */
extern const int HK_MEMORY_CLASS_UTILITIES;

/* Havok SDK boundary -- not reversed.
   hkdGeometry::setFaceIdentifiersToBestMatchingTriangle: this=geometry, then the
   reference vertices/indices, the per-section vertex/triangle counts (passed by
   reference), the output TriangleMap, and a match tolerance. */
extern void hkdGeometry_setFaceIdentifiersToBestMatchingTriangle(
    hkdGeometry *geometry,
    const hkVector4 *referenceVertices,
    const int *referenceTriangleIndices,
    const hkArray<int> *numVerticesPerSection,
    const hkArray<int> *numTrianglesPerSection,
    hkGeometryMatchingUtils_TriangleMap *triangleMap,
    float tolerance); /* hkdGeometry::setFaceIdentifiersToBestMatchingTriangle */

/* Sibling (src/havok/fixupChild.c): copies FaceIdentifier user-data from the root
   geometries into every face of a breakable-shape child tree. */
extern void fixupChild(const hkdBreakableShape *shape, hkArray<> *rootGeometries);

/* Allocate and default-construct an empty hkGeometryMatchingUtils::TriangleMap.
   DEVIATION: the decompile inlines the fixed-size free-list fast path
   (hkThreadMemory bucket 14 pop, with onRowEmpty as the slow path) followed by
   the TriangleMap constructor; modelled here as the equivalent allocateChunk plus
   the two hkInplaceArray inits. Each inplace array points m_data at its inline
   storage and stores capacity|DONT_DEALLOCATE (0x80000000) in m_capacityAndFlags. */
static hkGeometryMatchingUtils_TriangleMap *newTriangleMap(void)
{
    hkGeometryMatchingUtils_TriangleMap *triangleMap =
        (hkGeometryMatchingUtils_TriangleMap *)hkThreadMemory_allocateChunk(
            hkThreadMemory_getCurrent(),
            sizeof(hkGeometryMatchingUtils_TriangleMap),
            HK_MEMORY_CLASS_UTILITIES);
    if (!triangleMap)
        return 0;

    triangleMap->m_startIndexPerGeometry.m_data = triangleMap->m_startIndexPerGeometry.m_storage;
    triangleMap->m_startIndexPerGeometry.m_size = 0;
    triangleMap->m_startIndexPerGeometry.m_capacityAndFlags = 0x80000010; /* 16 | DONT_DEALLOCATE */

    triangleMap->m_foundReferenceTriangle.m_data = triangleMap->m_foundReferenceTriangle.m_storage;
    triangleMap->m_foundReferenceTriangle.m_size = 0;
    triangleMap->m_foundReferenceTriangle.m_capacityAndFlags = 0x80000080; /* 128 | DONT_DEALLOCATE */

    return triangleMap;
}

/* Rebuild the geometry<->reference triangle correspondence for a base breakable
   shape and each of its split-plane pieces after a fracture/rebuild, then re-link
   every child shape's face identifiers to the collected root geometries.
   Pairs with matchGraphicsToGeometry (0x8383FF50). */
void rematchGraphics(
    hkdAssetProcessingUtil_BaseObjectSource *baseObjectSource,
    hkdAssetProcessingUtil_SplitPlaneSources *splitPlaneSources)
{
    /* Base object: build its triangle map on first use, then match its geometry. */
    if (!baseObjectSource->m_triangleMap)
    {
        hkGeometryMatchingUtils_TriangleMap *triangleMap = newTriangleMap();
        baseObjectSource->m_triangleMap = triangleMap;
        hkdGeometry_setFaceIdentifiersToBestMatchingTriangle(
            (hkdGeometry *)baseObjectSource->m_breakableShape->m_geometry.m_pntr, /* DB param is non-const; m_geometry holds const hkdGeometry* */
            baseObjectSource->m_referenceVertices,
            baseObjectSource->m_referenceTriangleIndices,
            &baseObjectSource->m_numVerticesPerSection,
            &baseObjectSource->m_numTrianglesPerSection,
            triangleMap,
            0.2f);
    }

    /* Each split-plane geometry: same lazy map build + geometry match. */
    int splitCount = splitPlaneSources->m_geometries.m_size;
    for (int splitIndex = 0; splitIndex < splitCount; ++splitIndex)
    {
        hkGeometryMatchingUtils_TriangleMap **triangleMaps =
            (hkGeometryMatchingUtils_TriangleMap **)splitPlaneSources->m_triangleMaps.m_data;
        if (!triangleMaps[splitIndex])
        {
            triangleMaps[splitIndex] = newTriangleMap();
            hkdGeometry_setFaceIdentifiersToBestMatchingTriangle(
                ((hkdGeometry **)splitPlaneSources->m_geometries.m_data)[splitIndex],
                ((const hkVector4 **)splitPlaneSources->m_referenceVertices.m_data)[splitIndex],
                ((const int **)splitPlaneSources->m_referenceTriangleIndices.m_data)[splitIndex],
                ((const hkArray<int> **)splitPlaneSources->m_numVerticesPerSection.m_data)[splitIndex],
                ((const hkArray<int> **)splitPlaneSources->m_numTrianglesPerSection.m_data)[splitIndex],
                triangleMaps[splitIndex],
                0.2f);
        }
        splitCount = splitPlaneSources->m_geometries.m_size;
    }

    /* Collect the root geometries: base shape geometry first, then every split
       geometry. Backed by a freshly allocated pointer chunk. */
    int geometryCount = splitCount + 1;
    hkArray<> rootGeometries;
    rootGeometries.m_data = hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 4 * geometryCount, HK_MEMORY_CLASS_ARRAY);
    rootGeometries.m_size = geometryCount;
    rootGeometries.m_capacityAndFlags = geometryCount;

    hkdBreakableShape *baseShape = baseObjectSource->m_breakableShape;
    ((const hkdGeometry **)rootGeometries.m_data)[0] = baseShape->m_geometry.m_pntr;
    for (int geometryIndex = 1; geometryIndex < rootGeometries.m_size; ++geometryIndex)
        ((const hkdGeometry **)rootGeometries.m_data)[geometryIndex] =
            ((const hkdGeometry **)splitPlaneSources->m_geometries.m_data)[geometryIndex - 1];

    /* Re-link every child shape's faces to the collected root geometries. */
    for (int childIndex = 0; childIndex < baseShape->m_children.m_size; ++childIndex)
        fixupChild(((const hkdShapeInstanceInfo *)baseShape->m_children.m_data)[childIndex].m_shape,
                   &rootGeometries);

    if (rootGeometries.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), rootGeometries.m_data,
                                       4 * rootGeometries.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
}
