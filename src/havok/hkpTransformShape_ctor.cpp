// FUNCTION_INDEX entry: hkpTransformShape_ctor @0x83613FB0 (??0hkpTransformShape@@QAA@PBVhkpShape@@ABVhkTransform@@@Z)
#include "../headers/havok/hkpTransformShape.h"
#include "../headers/havok/hkpTransformShapeLayout.h"
#include "../headers/havok/hkpShapeType.h"
#include "../headers/havok/hkQuaternion.h"

/* Vtables installed by the constructor (boundary data). */
extern void *hkpTransformShape_vftable[];        /* hkpTransformShape::`vftable' */
extern void *hkpSingleShapeContainer_vftable[];  /* hkpSingleShapeContainer::`vftable' */

/* Boundary callees (2-level descent). */
extern void hkReferencedObject_addReference(hkReferencedObject *self);        /* hkReferencedObject::addReference */
extern void hkQuaternion_set(hkQuaternion *self, const hkRotation *rotation);  /* hkQuaternion::set(hkRotation) */

/* hkpTransformShape::hkpTransformShape(childShape, transform) — general transform
   wrapper shape. Installs the shape and container vtables, tags the shape type as
   HK_SHAPE_TRANSFORM (15), adopts childShape (adding a reference), copies the
   rigid transform into m_transform, and derives the quaternion m_rotation from
   the transform's rotation matrix.

   Also referenced under the flattened name hkpTransformShape_ctor (same
   compiled function, returning `this`) from hkdCompoundBreakableShape.

   DEVIATION: the 64-byte transform copy is a VMX128 lvx128/stvx128 block in the
   DB; reproduced here as a structure copy over the four 16-byte lanes. */
extern "C" void hkpTransformShape_ctor(hkpTransformShape *selfOpaque, const hkpShape *childShape,
                            const hkTransform *transform)
{
    hkpTransformShapeFull *self = (hkpTransformShapeFull *)selfOpaque;

    self->base.__vftable = (hkBaseObject_vtbl *)hkpTransformShape_vftable;
    self->base.m_referenceCount = 1;
    self->m_userData = 0;
    self->m_type.m_storage = HK_SHAPE_TRANSFORM;
    self->m_childShape.m_childShape = childShape;
    self->m_childShape.__vftable = (hkpSingleShapeContainer_vtbl *)hkpSingleShapeContainer_vftable;
    hkReferencedObject_addReference((hkReferencedObject *)childShape);

    self->m_transform = *transform;
    /* m_rotation is laid out as an hkVector4 (see hkpTransformShapeLayout.h) but is
       an hkQuaternion; cast to the type the callee expects. */
    hkQuaternion_set((hkQuaternion *)&self->m_rotation, &self->m_transform.m_rotation);
}
