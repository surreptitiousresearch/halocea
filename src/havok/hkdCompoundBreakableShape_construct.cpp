#include "../headers/havok/hkdCompoundBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkInplaceArray.h"
#include "../headers/havok/hkBaseObject.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkReferencedObject.h"

#define HK_REAL_MAX 3.402823466e38f

#include "../headers/havok/hkpListShape.h" // full layout (m_flags@36); was an opaque fwd typedef (2026-07-31)

/* Boundary callees/globals (2-level descent limit). */
extern void hkdBreakableShape_construct(hkdBreakableShape *self, const hkpShape *physicsShape, const void *parentName, const void *geometry);
extern hkBaseObject_vtbl hkdCompoundBreakableShape_vftable; /* hkdCompoundBreakableShape::`vftable' */
extern const hkpShape *hkdCompoundBreakableShape_addExtraTransformShape(hkdCompoundBreakableShape *self, const hkpShape *shape, const hkTransform *transform);
extern hkBool *hkTransform_isApproximatelyEqual(hkBool *out, const hkTransform *a, const hkTransform *b, float tolerance);
extern const hkTransform hkTransformIdentity;
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);
extern void hkpListShape_construct(hkpListShape *self, const hkpShape *const *shapes, int numShapes, int referencePolicy);
// REFERENCE_POLICY_INCREMENT now comes from hkpListShape.h's hkpShapeContainerReferencePolicy enum (2026-07-31)
extern const int HK_MEMORY_CLASS_CDINFO;
extern void hkdCompoundBreakableShape_recalcMassPropertiesFromChildren(hkdCompoundBreakableShape *self);
extern void hkdBreakableShape_setSimpleValuesFromParent(hkdBreakableShape *self, const hkdBreakableShape *oldParent);
extern void hkdBreakableShape_setSimpleValuesFromChildren(hkdBreakableShape *self, const hkdShapeInstanceInfo *children, int numChildren);

/* hkdCompoundBreakableShape::hkdCompoundBreakableShape — assemble a compound
   breakable shape from a set of child instance infos. Children whose placement
   is non-identity (or when the CTR flag bit0 forces it) are wrapped in an extra
   transform shape; the collected physics shapes are gathered into an
   hkpListShape used as this shape's physics representation. */
void hkdCompoundBreakableShape_construct(hkdCompoundBreakableShape *self, const hkdBreakableShape *oldParent,
        hkdShapeInstanceInfo *childBreakableShapes, int numChildBreakableShapes, int flags)
{
    hkInplaceArray<const hkpShape *, 8> shapes;
    hkpListShape *listShape;
    const hkpShape *oldPhysics;
    int i;

    hkdBreakableShape_construct(&self->base, 0, 0, 0);
    ((hkBaseObject *)self)->__vftable = &hkdCompoundBreakableShape_vftable;
    self->m_useChildrenBreakableThreshold.m_bool = 0;
    self->base.m_type.m_storage = SHAPE_TYPE_COMPOUND;
    self->m_rootBreakableShape = 0;

    shapes.m_data = shapes.m_storage;
    shapes.m_size = 0;
    shapes.m_capacityAndFlags = (int)0x80000008; /* inline capacity 8, storage not owned */

    for (i = 0; i < numChildBreakableShapes; ++i)
    {
        hkdShapeInstanceInfo *child = &childBreakableShapes[i];
        const hkpShape *physics = child->m_shape->m_physicsShape.m_pntr;
        hkBool eq;
        int needsTransform = 0;

        if ((flags & 1) != 0
            || !hkTransform_isApproximatelyEqual(&eq, &child->m_transform, &hkTransformIdentity, 0.001f)->m_bool)
            needsTransform = 1;

        if (needsTransform)
        {
            const hkpShape *wrapped = hkdCompoundBreakableShape_addExtraTransformShape(self, physics, &child->m_transform);
            if (shapes.m_size == (shapes.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&shapes.m_data, 4);
            shapes.m_data[shapes.m_size++] = wrapped;
        }
        else
        {
            if (shapes.m_size == (shapes.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&shapes.m_data, 4);
            shapes.m_data[shapes.m_size++] = physics;
            hkReferencedObject_addReference((hkReferencedObject *)physics);
        }
    }

    listShape = (hkpListShape *)hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 112, HK_MEMORY_CLASS_CDINFO);
    ((hkReferencedObject *)listShape)->m_memSizeAndFlags = 112;
    hkpListShape_construct(listShape, shapes.m_data, shapes.m_size, REFERENCE_POLICY_INCREMENT);

    /* release the temporary references now held by the list shape */
    for (i = 0; i < shapes.m_size; ++i)
        hkReferencedObject_removeReference((hkReferencedObject *)shapes.m_data[i]);

    /* typed member (DB types_members hkpListShape::m_flags @36; was a raw +36 poke) */
    listShape->m_flags |= 1u;

    oldPhysics = self->base.m_physicsShape.m_pntr;
    if (oldPhysics)
        hkReferencedObject_removeReference((hkReferencedObject *)oldPhysics);
    self->base.m_physicsShape.m_pntr = (const hkpShape *)listShape;

    if ((flags & 2) != 0)
    {
        for (i = 0; i < numChildBreakableShapes; ++i)
            ((hkdBreakableShape *)childBreakableShapes[i].m_shape)->m_parent = &self->base;
    }
    else
    {
        const hkdBreakableShape *parent = childBreakableShapes[0].m_shape->m_parent;
        self->m_rootBreakableShape = parent;
        if (parent)
        {
            while (self->m_rootBreakableShape->m_parent)
                self->m_rootBreakableShape = self->m_rootBreakableShape->m_parent;
            hkReferencedObject_addReference((hkReferencedObject *)self->m_rootBreakableShape);
        }
    }

    /* append every child instance to m_children (without touching m_parent) */
    for (i = 0; i < numChildBreakableShapes; ++i)
    {
        hkdShapeInstanceInfo *src = &childBreakableShapes[i];
        hkdShapeInstanceInfo *dst;
        if (self->base.m_children.m_size == (self->base.m_children.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&self->base.m_children.m_data, 80);
        dst = &self->base.m_children.m_data[self->base.m_children.m_size++];
        dst->m_transform = src->m_transform;
        dst->m_shape = src->m_shape;
        dst->m_damage = src->m_damage;
        dst->m_runtimeInfo = src->m_runtimeInfo;
        hkReferencedObject_addReference((hkReferencedObject *)src->m_shape);
    }

    hkdCompoundBreakableShape_recalcMassPropertiesFromChildren(self);

    if (oldParent)
    {
        if (oldParent->m_type.m_storage == SHAPE_TYPE_COMPOUND)
            self->m_useChildrenBreakableThreshold.m_bool =
                ((const hkdCompoundBreakableShape *)oldParent)->m_useChildrenBreakableThreshold.m_bool;
        hkdBreakableShape_setSimpleValuesFromParent(&self->base, oldParent);
    }
    else
    {
        hkdBreakableShape_setSimpleValuesFromChildren(&self->base, childBreakableShapes, numChildBreakableShapes);
    }

    /* free the scratch shape array if it spilled to the heap */
    if ((shapes.m_capacityAndFlags & 0x80000000) == 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), shapes.m_data,
                                       4 * (shapes.m_capacityAndFlags & 0x3FFFFFFF), HK_MEMORY_CLASS_ARRAY);
}
