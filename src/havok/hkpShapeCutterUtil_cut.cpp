// hkpShapeCutterUtil_cut @0x83F97520
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkpShapeType.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkArray.h"

/* Opaque shape subclasses (level-2 boundary — only handled through pointers). */
typedef struct hkpConvexVerticesShape hkpConvexVerticesShape;
typedef struct hkpListShape hkpListShape;
typedef struct hkpConvexTransformShape hkpConvexTransformShape;
typedef struct hkpTransformShape hkpTransformShape;

/* boundary — per-primitive convex-hull builders, the plane cut, and shape wrappers.
   DEVIATION: prior text declared a fictional 3-arg `hkpShapeCutterUtil_cut3`
   forwarder for the recursion and passed shape pointers into approxSphereRows /
   createSphereConvexVerticesShape; the binary (0x83F97520) recurses directly into
   itself (3-arg calls @0x83F97708/0x83F97820/0x83F97904/0x83F97AFC) and passes
   (1.0f, 256, radius) / (radius, rows). */
extern int  hkpShapeCutterUtil_approxSphereRows(float edgeSize, int maxFaces, float radius);
extern hkpConvexVerticesShape *hkpShapeCutterUtil_createSphereConvexVerticesShape(float radius, int numRowsIn);
extern hkpConvexVerticesShape *hkpShapeCutterUtil_createCylinderConvexVerticesShape(float radius, const hkVector4 *vertexA, const hkVector4 *vertexB, int numSides);
extern hkpConvexVerticesShape *hkpShapeCutterUtil_createAabbConvexVerticesShape(const hkAabb *aabb);
extern hkpConvexVerticesShape *hkpShapeCutterUtil_createCapsuleConvexVerticesShape(const hkVector4 *top, const hkVector4 *bottom, float radius, int numSides, int numHeightSegments);
extern const hkpConvexVerticesShape *hkpConvexVerticesConnectivityUtil_cut(hkpConvexVerticesShape *shape, const hkVector4 *plane, float radius, float tolerance);
extern int  hkpConvexVerticesConnectivityUtil_ensureConnectivity(const hkpConvexVerticesShape *shape);
extern void hkpConvexVerticesShape_transformVerticesAndPlaneEquations(hkpConvexVerticesShape *shape, const hkTransform *transform);
extern void hkpShapeCutterUtil_inverseTransformPlane(const hkVector4 *plane, const hkTransform *transform, hkVector4 *out);
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);
extern void hkReferencedObject_removeReferences(void *objects, int numObjects, int pointerStriding);

/* boundary — field/child accessors for the concrete shapes (the decompiler read
   these through a single hkpCylinderShape overlay; expressed here as accessors). */
extern float             hkpConvexShape_getRadius(const hkpShape *shape);
extern float             hkpCylinderShape_getCylinderRadius(const hkpShape *shape);
extern const hkVector4  *hkpCylinderShape_getVertex(const hkpShape *shape, int which);
/* getTransform takes a caller-provided per-frame scratch buffer: the CONVEX_TRANSLATE arm
   has no stored transform and synthesizes one, and cut's recursion needs that buffer to live
   on THIS frame (a shared static would be clobbered by a nested CONVEX_TRANSLATE). */
extern const hkTransform *hkpWrapperShape_getTransform(const hkpShape *shape, hkTransform *scratch);
extern const hkpShape    *hkpWrapperShape_getChildShape(const hkpShape *shape);
extern int                hkpListShape_getNumChildShapes(const hkpShape *shape);
extern const hkpShape    *hkpListShape_getChildShapeInl(const hkpShape *shape, int index); /* hkpListShape::getChildShapeInl (the binary's inlined child fetch) */
extern void hkpConvexVerticesShape_setRadius(hkpConvexVerticesShape *shape, float r);
extern hkpListShape           *hkpListShape_create(const hkpShape *const *shapes, int count);
extern hkpConvexTransformShape *hkpConvexTransformShape_create(const hkpConvexVerticesShape *child, const hkTransform *t);
extern hkpTransformShape       *hkpTransformShape_create(const hkpConvexVerticesShape *child, const hkTransform *t);

/* Finish a primitive case: temp has already had one reference removed. If the
   connectivity cut returned the temp unchanged (plane does not intersect), drop
   it and hand back the original shape (referenced); otherwise return the cut. */
static const hkpConvexVerticesShape *finish_primitive(const hkpConvexVerticesShape *result,
                                                      hkpConvexVerticesShape *temp,
                                                      const hkpShape *shapeIn)
{
    if (result == temp)
    {
        hkReferencedObject_removeReference((hkReferencedObject *)temp);
        hkReferencedObject_addReference((hkReferencedObject *)shapeIn);
        return (const hkpConvexVerticesShape *)shapeIn;
    }
    return result;
}

/* Wrap the cut child of a transform-family shape: if the child came back as a
   convex-vertices shape, bake the transform into its vertices and return it;
   otherwise return it under a fresh transform wrapper. Shared by the
   CONVEX_TRANSLATE / CONVEX_TRANSFORM / TRANSFORM cases. */
static const hkpConvexVerticesShape *wrap_transformed(const hkpShape *child,
                                                      const hkpShape *cutChild,
                                                      const hkTransform *transform,
                                                      const hkpShape *shapeIn,
                                                      int useConvexWrapper)
{
    if (cutChild == child)
    {
        hkReferencedObject_removeReference((hkReferencedObject *)cutChild);
        hkReferencedObject_addReference((hkReferencedObject *)shapeIn);
        return (const hkpConvexVerticesShape *)shapeIn;
    }
    if (!cutChild)
        return 0;
    if (((const hkpShape *)cutChild)->m_type.m_storage == HK_SHAPE_CONVEX_VERTICES)
    {
        hkpConvexVerticesShape_transformVerticesAndPlaneEquations(
            (hkpConvexVerticesShape *)cutChild, transform);
        return (const hkpConvexVerticesShape *)cutChild;
    }
    {
        const hkpConvexVerticesShape *r;
        if (useConvexWrapper)
            r = (const hkpConvexVerticesShape *)hkpConvexTransformShape_create(
                    (const hkpConvexVerticesShape *)cutChild, transform);
        else
            r = (const hkpConvexVerticesShape *)hkpTransformShape_create(
                    (const hkpConvexVerticesShape *)cutChild, transform);
        hkReferencedObject_removeReference((hkReferencedObject *)cutChild);
        return r;
    }
}

/* hkpShapeCutterUtil::cut — cut a shape by a plane, returning a (referenced) new
   shape holding the negative half-space, the original shape (referenced) when
   the plane misses it, or null when the plane removes it entirely. Convex
   primitives are tessellated into a convex-vertices hull and cut; list shapes
   cut each child and rebuild only if something changed; transform-family shapes
   pull the plane into local space, cut the child, and re-wrap the result.
   DEVIATION: the optimized binary overlays every shape type on a single
   hkpCylinderShape view and inlines the per-thread stack allocator; the
   concrete-field reads are expressed here through boundary accessors and the
   recursion is the direct self-call the binary makes. */
extern "C" const hkpConvexVerticesShape *hkpShapeCutterUtil_cut(hkpShape *shapeIn, const hkVector4 *plane,
                                                     float extraConvexRadiusForImplicitShapes)
{
    switch (shapeIn->m_type.m_storage)
    {
        case HK_SHAPE_SPHERE:
        {
            float radius = hkpConvexShape_getRadius(shapeIn);
            float planeDist = plane->m_quad.___u0.v[3];
            hkpConvexVerticesShape *temp;
            const hkpConvexVerticesShape *result;
            int rows;
            if (planeDist < -radius)
            {
                hkReferencedObject_addReference((hkReferencedObject *)shapeIn);
                return (const hkpConvexVerticesShape *)shapeIn; /* fully inside */
            }
            if (planeDist > radius)
                return 0; /* fully cut away */
            rows = hkpShapeCutterUtil_approxSphereRows(1.0f, 256, radius);
            if (rows < 4)
                rows = 4; /* DEVIATION: restored the binary's >=4 clamp (li r4,4 @0x83F9767C) */
            temp = hkpShapeCutterUtil_createSphereConvexVerticesShape(radius, rows);
            hkpConvexVerticesShape_setRadius(temp, extraConvexRadiusForImplicitShapes);
            result = hkpConvexVerticesConnectivityUtil_cut(temp, plane, extraConvexRadiusForImplicitShapes, 0.00000099999988f);
            hkReferencedObject_removeReference((hkReferencedObject *)temp);
            return finish_primitive(result, temp, shapeIn);
        }

        case HK_SHAPE_CYLINDER:
        {
            float radius = hkpCylinderShape_getCylinderRadius(shapeIn);
            /* DEVIATION: numSides is the binary's constant 20 (li r6,0x14 @0x83F975EC);
               prior text routed a phantom cut() parameter here. */
            hkpConvexVerticesShape *temp = hkpShapeCutterUtil_createCylinderConvexVerticesShape(
                radius, hkpCylinderShape_getVertex(shapeIn, 0), hkpCylinderShape_getVertex(shapeIn, 1), 20);
            const hkpConvexVerticesShape *result;
            hkpConvexVerticesShape_setRadius(temp, extraConvexRadiusForImplicitShapes);
            result = hkpConvexVerticesConnectivityUtil_cut(temp, plane, extraConvexRadiusForImplicitShapes, 0.00000099999988f);
            hkReferencedObject_removeReference((hkReferencedObject *)temp);
            return finish_primitive(result, temp, shapeIn);
        }

        case HK_SHAPE_BOX:
        {
            float radius = hkpConvexShape_getRadius(shapeIn);
            hkAabb aabb;
            hkpConvexVerticesShape *temp;
            const hkpConvexVerticesShape *result;
            const hkVector4 *halfExtents = hkpCylinderShape_getVertex(shapeIn, 0); /* box half-extents overlay */
            int i;
            for (i = 0; i < 4; ++i)
            {
                aabb.m_max.m_quad.___u0.v[i] = halfExtents->m_quad.___u0.v[i];
                aabb.m_min.m_quad.___u0.v[i] = -halfExtents->m_quad.___u0.v[i];
            }
            temp = hkpShapeCutterUtil_createAabbConvexVerticesShape(&aabb);
            hkpConvexVerticesShape_setRadius(temp, radius);
            result = hkpConvexVerticesConnectivityUtil_cut(temp, plane, radius, 0.00000099999988f);
            hkReferencedObject_removeReference((hkReferencedObject *)temp);
            return finish_primitive(result, temp, shapeIn);
        }

        case HK_SHAPE_CAPSULE:
        {
            float radius = hkpConvexShape_getRadius(shapeIn);
            /* DEVIATION: the binary passes numSides=6, numHeightSegments=1
               (li r6,6 / li r7,1 @0x83F979F8..0x83F97A00); prior text routed a
               phantom cut() parameter into the 4th slot. */
            hkpConvexVerticesShape *temp = hkpShapeCutterUtil_createCapsuleConvexVerticesShape(
                hkpCylinderShape_getVertex(shapeIn, 0), hkpCylinderShape_getVertex(shapeIn, 1), radius, 6, 1);
            const hkpConvexVerticesShape *result;
            hkpConvexVerticesShape_setRadius(temp, extraConvexRadiusForImplicitShapes);
            result = hkpConvexVerticesConnectivityUtil_cut(temp, plane, radius, 0.00000099999988f);
            hkReferencedObject_removeReference((hkReferencedObject *)temp);
            return finish_primitive(result, temp, shapeIn);
        }

        case HK_SHAPE_CONVEX_VERTICES:
        {
            if (hkpConvexVerticesConnectivityUtil_ensureConnectivity((const hkpConvexVerticesShape *)shapeIn))
                return 0;
            return hkpConvexVerticesConnectivityUtil_cut(
                (hkpConvexVerticesShape *)shapeIn, plane, hkpConvexShape_getRadius(shapeIn), 0.00000099999988f);
        }

        case HK_SHAPE_LIST:
        {
            int n = hkpListShape_getNumChildShapes(shapeIn);
            hkArray<void> cutChildren;
            const hkpShape *result = 0;
            int changed = 0;
            int i;

            if (n > 256)
                return 0;
            cutChildren.m_data = 0;
            cutChildren.m_size = 0;
            cutChildren.m_capacityAndFlags = 0x80000000;

            for (i = 0; i < n; ++i)
            {
                const hkpShape *child = hkpListShape_getChildShapeInl(shapeIn, i);
                const hkpShape *cutChild = (const hkpShape *)hkpShapeCutterUtil_cut((hkpShape *)child, plane, 0.001f);
                if (!cutChild)
                {
                    changed = 1;
                    continue;
                }
                if (cutChildren.m_size == (cutChildren.m_capacityAndFlags & 0x3FFFFFFF))
                    hkArrayUtil__reserveMore(&cutChildren, 4);
                ((const hkpShape **)cutChildren.m_data)[cutChildren.m_size++] = cutChild;
                if (cutChild != child)
                    changed = 1;
            }

            if (cutChildren.m_size > 0)
            {
                if (cutChildren.m_size == 1)
                {
                    result = ((const hkpShape **)cutChildren.m_data)[0];
                    hkReferencedObject_addReference((hkReferencedObject *)result);
                }
                else if (changed)
                {
                    result = (const hkpShape *)hkpListShape_create(
                        (const hkpShape *const *)cutChildren.m_data, cutChildren.m_size);
                    hkReferencedObject_removeReferences(
                        cutChildren.m_data, cutChildren.m_size, 4);
                    goto list_cleanup;
                }
                else
                {
                    result = shapeIn;
                    hkReferencedObject_addReference((hkReferencedObject *)result);
                }
                hkReferencedObject_removeReferences(
                    cutChildren.m_data, cutChildren.m_size, 4);
            }

        list_cleanup:
            if (cutChildren.m_capacityAndFlags >= 0)
                hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), cutChildren.m_data,
                                               4 * cutChildren.m_capacityAndFlags, HK_MEMORY_CLASS_ARRAY);
            return (const hkpConvexVerticesShape *)result;
        }

        case HK_SHAPE_CONVEX_TRANSLATE:
        case HK_SHAPE_CONVEX_TRANSFORM:
        case HK_SHAPE_TRANSFORM:
        {
            const hkpShape *child = hkpWrapperShape_getChildShape(shapeIn);
            hkTransform xformBuf;   /* per-frame buffer for the synthesized CONVEX_TRANSLATE transform (binary var_90) */
            const hkTransform *transform = hkpWrapperShape_getTransform(shapeIn, &xformBuf);
            hkVector4 localPlane;
            const hkpShape *cutChild;
            int useConvexWrapper = (shapeIn->m_type.m_storage != HK_SHAPE_TRANSFORM);
            if (!child)
                return 0;
            hkpShapeCutterUtil_inverseTransformPlane(plane, transform, &localPlane);
            cutChild = (const hkpShape *)hkpShapeCutterUtil_cut((hkpShape *)child, &localPlane, 0.001f);
            return wrap_transformed(child, cutChild, transform, shapeIn, useConvexWrapper);
        }

        default:
            return 0;
    }
}
