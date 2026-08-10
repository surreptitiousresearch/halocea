#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkGeomConvexHull.h"
#include "../headers/havok/hkThreadMemory.h"

/* The free-function worker (?_findConnectivity@@YA...@Z @0x83F92E38, sourced in
   src/havok/hkpConvexVerticesConnectivityUtil__findConnectivity_impl.cpp). Builds
   the connectivity object from the original vertices, plane equations, hull
   vertices, and hull edge structure. DEVIATION: a prior extern spelled this
   `hkpConvexVerticesConnectivityUtil__findConnectivity`, colliding with the flat
   name of the 1-arg member findConnectivity. */
extern hkpConvexVerticesConnectivity *hkpConvexVerticesConnectivityUtil__findConnectivity_impl(
        const hkArray<hkVector4> *vertices, const hkArray<hkVector4> *facePlanes,
        const hkArray<hkVector4> *usedVertices, hkGeomHull *hull); /* ::_findConnectivity */

/* hkpConvexVerticesConnectivityUtil::findConnectivity @ 0x83F937B0
   Rebuild a shape's per-vertex adjacency: extract its original vertices and plane
   equations, run the convex-hull builder, hand both to the private worker, then
   release the three working buffers (edge array, hull vertices, original
   vertices) that were not consumed.

   Both hkArray locals start empty with only the DONT_DEALLOCATE sign bit set
   (`li r31,0` / `lis r30,-0x8000` @0x83F937BC); the only inline storage is the
   hull's own hkInplaceArray edge buffer. The three frees use the element sizes
   the binary shifts by: 8 for hkGeomEdge (`slwi r5,r11,3` @0x83F9384C) and 16 for
   hkVector4 (`slwi r5,r11,4` @0x83F93878/0x83F938A4). */
hkpConvexVerticesConnectivity *hkpConvexVerticesConnectivityUtil_findConnectivity(hkpConvexVerticesShape *shape)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    const hkArray<hkVector4> *planeEquations;
    hkpConvexVerticesConnectivity *connectivity;
    hkArray<hkVector4> originalVertices;
    hkArray<hkVector4> hullVertices;
    hkGeomHull hull;

    originalVertices.m_data = nullptr;
    originalVertices.m_size = 0;
    originalVertices.m_capacityAndFlags = 0x80000000;
    hkpConvexVerticesShape_getOriginalVertices(shape, &originalVertices);

    planeEquations = hkpConvexVerticesShape_getPlaneEquations(shape);

    hullVertices.m_data = nullptr;
    hullVertices.m_size = 0;
    hullVertices.m_capacityAndFlags = 0x80000000;

    hkGeomHull_construct(&hull);
    hkGeomConvexHullBuilder_generateConvexHull(originalVertices.m_data, originalVertices.m_size,
                                               &hull, &hullVertices, HK_GEOM_CONVEXHULL_MODE_FAST);

    connectivity = hkpConvexVerticesConnectivityUtil__findConnectivity_impl(&originalVertices, planeEquations,
                                                                       &hullVertices, &hull);

    if (hull.m_edges.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, hull.m_edges.m_data,
                                       8 * hull.m_edges.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (hullVertices.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, hullVertices.m_data,
                                       16 * hullVertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
    if (originalVertices.m_capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(mem, originalVertices.m_data,
                                       16 * originalVertices.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);

    return connectivity;
}
