#include "../headers/havok/hkdCompoundBreakableShape.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkpShapeType.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkReferencedObject.h"

/* Opaque wrapper shapes (boundary — only allocated and constructed here). */
typedef struct hkpTransformShape hkpTransformShape;
typedef struct hkpConvexTransformShape hkpConvexTransformShape;

extern const int HK_MEMORY_CLASS_CDINFO;
extern const int REFERENCE_POLICY_INCREMENT;
/* Both ctor shims return void per their real DEFs; each branch returns the
   (first-arg) wrapper it constructed. */
extern void hkpConvexTransformShape_construct(hkpConvexTransformShape *self,
        const hkpShape *childShape, const hkTransform *transform, int referencePolicy); /* hkpConvexTransformShape::hkpConvexTransformShape */
extern void hkpTransformShape_ctor(hkpTransformShape *self,
        const hkpShape *childShape, const hkTransform *transform); /* hkpTransformShape::hkpTransformShape */

/* hkdCompoundBreakableShape::addExtraTransformShape — wrap physicsShape in a
   transform shape carrying `transform`. Convex primitives use the compact
   hkpConvexTransformShape (96 bytes); every other shape kind uses a general
   hkpTransformShape (112 bytes).
   DEVIATION: the decompiler types the return void, but both branches leave the
   freshly constructed wrapper in r3 (the constructors return `this`) and the
   caller (hkdCompoundBreakableShape::hkdCompoundBreakableShape) consumes it, so
   the wrapper is returned here. `self` is unreferenced, matching the DB body. */
const hkpShape *hkdCompoundBreakableShape_addExtraTransformShape(hkdCompoundBreakableShape *self,
        const hkpShape *physicsShape, const hkTransform *transform)
{
    unsigned int shapeType = physicsShape->m_type.m_storage;

    (void)self;

    if (shapeType == HK_SHAPE_CONVEX
        || shapeType == HK_SHAPE_SPHERE
        || shapeType == HK_SHAPE_CYLINDER
        || shapeType == HK_SHAPE_TRIANGLE
        || shapeType == HK_SHAPE_BOX
        || shapeType == HK_SHAPE_CAPSULE
        || shapeType == HK_SHAPE_CONVEX_VERTICES
        || shapeType == HK_SHAPE_CONVEX_TRANSLATE
        || shapeType == HK_SHAPE_CONVEX_TRANSFORM
        || shapeType == HK_SHAPE_CONVEX_PIECE)
    {
        hkpConvexTransformShape *wrapper = (hkpConvexTransformShape *)hkThreadMemory_allocateChunk(
            hkThreadMemory_getCurrent(), 96, HK_MEMORY_CLASS_CDINFO);
        ((hkReferencedObject *)wrapper)->m_memSizeAndFlags = 96;
        /* construct returns void; the constructed wrapper (its first arg) is returned. */
        hkpConvexTransformShape_construct(wrapper, physicsShape, transform, REFERENCE_POLICY_INCREMENT);
        return (const hkpShape *)wrapper;
    }
    else
    {
        hkpTransformShape *wrapper = (hkpTransformShape *)hkThreadMemory_allocateChunk(
            hkThreadMemory_getCurrent(), 112, HK_MEMORY_CLASS_CDINFO);
        ((hkReferencedObject *)wrapper)->m_memSizeAndFlags = 112;
        hkpTransformShape_ctor(wrapper, physicsShape, transform);
        return (const hkpShape *)wrapper;
    }
}
