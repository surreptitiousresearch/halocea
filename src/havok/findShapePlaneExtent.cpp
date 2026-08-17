/* findShapePlaneExtent @0x83877D80 */
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkResult.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkShapeExtractor_boundary.h" /* hkpShape, hkpConvexShape, flattenIntoConvexShapes */

/* findShapesSupport: sibling (src/havok/findShapesSupport.c) — GJK support map
   over the flattened convex pieces; writes the world-space supporting vertex. */
extern void findShapesSupport(const hkArray<> *shapes, const hkArray<> *transforms,
                              const hkVector4 *direction, hkVector4 *supportOut);

/* Signed distance of `point` to `plane` (plane = {nx,ny,nz,d}): dot3(n,point)+d.
   DEVIATION: original is VMX128 -- vmsum3fp128 (three-lane dot of plane*point)
   then vaddfp with vspltw(plane,3) (the plane's w/d lane splatted); one lane is
   stored via stvewx. Reconstructed as scalar. */
static float signedDistanceToPlane(const hkVector4 *plane, const hkVector4 *point)
{
    return plane->m_quad.___u0.v[0] * point->m_quad.___u0.v[0]
         + plane->m_quad.___u0.v[1] * point->m_quad.___u0.v[1]
         + plane->m_quad.___u0.v[2] * point->m_quad.___u0.v[2]
         + plane->m_quad.___u0.v[3];
}

/* Computes the extent (support slab) of a shape along a plane normal: flattens
   the shape into convex pieces, then finds the supporting vertex in the +normal
   and -normal directions and returns each vertex's signed distance to the plane
   as *max / *min. Returns the flatten failure code, or HK_SUCCESS. */
hkResult findShapePlaneExtent(const hkpShape *shape,
                              const hkTransform *shapeTransform,
                              const hkVector4 *plane,
                              float *max,
                              float *min)
{
    hkArray<> convexShapes; /* hkArray<const hkpConvexShape*> */
    hkArray<> transforms;   /* hkArray<hkTransform>          */
    hkResult flattenResult;

    convexShapes.m_data = 0;
    convexShapes.m_size = 0;
    convexShapes.m_capacityAndFlags = 0x80000000; /* stack-owned: dont-deallocate */

    transforms.m_data = 0;
    transforms.m_size = 0;
    transforms.m_capacityAndFlags = 0x80000000;

    flattenResult = hkpShapeCutterUtil_flattenIntoConvexShapes(shape, shapeTransform,
                                                               &transforms, &convexShapes);
    if (flattenResult)
    {
        /* Flatten failed: report a degenerate slab and release any heap growth. */
        *max = 0.0f;
        *min = 0.0f;

        if ((int)transforms.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), transforms.m_data,
                                           (transforms.m_capacityAndFlags & 0x3FFFFFFF) << 6, /* hkTransform = 64B */
                                           HK_MEMORY_CLASS_ARRAY);
        if ((int)convexShapes.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), convexShapes.m_data,
                                           4 * (convexShapes.m_capacityAndFlags & 0x3FFFFFFF), /* 4B pointers */
                                           HK_MEMORY_CLASS_ARRAY);
        return flattenResult;
    }

    {
        hkVector4 support;
        hkVector4 negatedPlane;

        /* Support in the +normal direction gives the far side of the slab. */
        findShapesSupport(&convexShapes, &transforms, plane, &support);
        *max = signedDistanceToPlane(plane, &support);

        /* Support in the -normal direction gives the near side. The direction is
           the fully negated plane (setNeg4 XORs all four lanes); the projection
           itself still uses the original plane. */
        negatedPlane.m_quad.___u0.v[0] = -plane->m_quad.___u0.v[0];
        negatedPlane.m_quad.___u0.v[1] = -plane->m_quad.___u0.v[1];
        negatedPlane.m_quad.___u0.v[2] = -plane->m_quad.___u0.v[2];
        negatedPlane.m_quad.___u0.v[3] = -plane->m_quad.___u0.v[3];
        findShapesSupport(&convexShapes, &transforms, &negatedPlane, &support);
        *min = signedDistanceToPlane(plane, &support);

        if ((transforms.m_capacityAndFlags & 0x80000000) == 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), transforms.m_data,
                                           (transforms.m_capacityAndFlags & 0x3FFFFFFF) << 6,
                                           HK_MEMORY_CLASS_ARRAY);
        if ((int)convexShapes.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), convexShapes.m_data,
                                           4 * (convexShapes.m_capacityAndFlags & 0x3FFFFFFF),
                                           HK_MEMORY_CLASS_ARRAY);
        return HK_SUCCESS;
    }
}
