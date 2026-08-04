#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkGeomConvexHull.h"
#include "../headers/havok/hkThreadMemory.h"

/* Level-1 boundary callees (bounded descent — kept extern). */
extern void hkpConvexVerticesShape_getOriginalVertices(hkpConvexVerticesShape *shape, hkArray<hkVector4> *verticesOut); /* hkpConvexVerticesShape::getOriginalVertices */
extern const hkArray<hkVector4> *hkpConvexVerticesShape_getPlaneEquations(hkpConvexVerticesShape *shape);               /* hkpConvexVerticesShape::getPlaneEquations */

/* The internal free-function worker (?_findConnectivity@@YA...@Z @0x83F92E38) that
   turns the vertices + plane equations + built hull into a connectivity object. */
extern hkpConvexVerticesConnectivity *hkpConvexVerticesConnectivityUtil__findConnectivity_impl(
        const hkArray<hkVector4> *vertices, const hkArray<hkVector4> *planeEquations,
        const hkArray<hkVector4> *usedVertices, hkGeomHull *hull); /* ::_findConnectivity */

/* hkpConvexVerticesConnectivityUtil::findConnectivity — build the face/vertex
   adjacency for a convex-vertices shape: pull the shape's original vertices and
   plane equations, generate a geometric hull over them (FAST mode), and hand all
   three to the internal _findConnectivity worker. The temporary vertex arrays and
   the hull's spilled edge storage are released before returning the new object.

   DEVIATION: the three working arrays use inline stack storage in the original
   (started empty with the DONT_DEALLOCATE sign bit); the hull edge free uses an
   8-byte element size (hkGeomEdge). Reconstructed faithfully. */
hkpConvexVerticesConnectivity *hkpConvexVerticesConnectivityUtil__findConnectivity(hkpConvexVerticesShape *shape)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    hkArray<hkVector4> vertices = { nullptr, 0, (int)0x80000000 };
    hkArray<hkVector4> usedVertices = { nullptr, 0, (int)0x80000000 };
    const hkArray<hkVector4> *planeEquations;
    hkGeomHull hull;
    hkpConvexVerticesConnectivity *result;

    hkpConvexVerticesShape_getOriginalVertices(shape, &vertices);
    planeEquations = hkpConvexVerticesShape_getPlaneEquations(shape);

    hkGeomHull_construct(&hull);
    hkGeomConvexHullBuilder_generateConvexHull(vertices.m_data, vertices.m_size, &hull, &usedVertices,
                                               HK_GEOM_CONVEXHULL_MODE_FAST);

    result = hkpConvexVerticesConnectivityUtil__findConnectivity_impl(&vertices, planeEquations, &usedVertices, &hull);

    if (hull.m_edges.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, hull.m_edges.m_data, 8 * hull.m_edges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (usedVertices.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, usedVertices.m_data, 16 * usedVertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (vertices.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, vertices.m_data, 16 * vertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);

    return result;
}
