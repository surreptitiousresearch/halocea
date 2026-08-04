#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkGeomHull.h"
#include "../headers/havok/hkThreadMemory.h"

extern void hkpConvexVerticesShape_getOriginalVertices(hkpConvexVerticesShape *shape, hkArray<hkVector4> *out); /* hkpConvexVerticesShape::getOriginalVertices */
extern const hkArray<hkVector4> *hkpConvexVerticesShape_getPlaneEquations(hkpConvexVerticesShape *shape);        /* hkpConvexVerticesShape::getPlaneEquations */

extern void hkGeomHull_construct(hkGeomHull *self);   /* hkGeomHull::hkGeomHull */

/* hkGeomConvexHullMode — DB types_enum_values. */
enum hkGeomConvexHullMode
{
    HK_GEOM_CONVEXHULL_MODE_INVALID            = 0,
    HK_GEOM_CONVEXHULL_MODE_FAST               = 1,
    HK_GEOM_CONVEXHULL_MODE_ACCURATE_BUT_SLOW  = 2,
    HK_GEOM_CONVEXHULL_MODE_MAX_ID             = 3,
};

extern void hkGeomConvexHullBuilder_generateConvexHull(const hkVector4 *vertices, int numVertices,
                                                       hkGeomHull *hullOut, hkArray<hkVector4> *hullVerticesOut,
                                                       enum hkGeomConvexHullMode mode); /* hkGeomConvexHullBuilder::generateConvexHull */

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
   vertices) that were not consumed. */
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
