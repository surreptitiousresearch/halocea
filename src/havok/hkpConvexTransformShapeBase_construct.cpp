#include "../headers/havok/hkpConvexTransformShapeBase.h"
#include "../headers/havok/hkReferencedObject.h"

/* hkpConvexTransformShapeBase::hkpConvexTransformShapeBase — construct the shared
   base of the convex transform/translate shapes: tag the shape type and radius,
   install the base vtable, seed the refcount, clear userData, and adopt the child
   shape through a single-shape container. Under REFERENCE_POLICY_INCREMENT (1) the
   child gets an added reference.

   DEVIATION: the decompiler split the single child pointer (r5) into two aliases
   (childShape / ref) and invented a transform parameter at the derived call site;
   the true ABI (mangled name) takes only (type, radius, childShape, refPolicy). */
void hkpConvexTransformShapeBase_construct(hkpConvexTransformShapeBase *self, int shapeType,
                                           float radius, const hkpShape *childShape,
                                           int referencePolicy)
{
    self->m_type.m_storage = (unsigned int)shapeType;
    self->m_radius = radius;
    *(void **)&self->base = (void *)hkpConvexTransformShapeBase_vftable;
    self->base.m_referenceCount = 1;
    self->m_userData = 0;
    self->m_childShape.__vftable = (void *)hkpSingleShapeContainer_vftable;
    self->m_childShape.m_childShape = childShape;
    if (referencePolicy == 1)
        hkReferencedObject_addReference((hkReferencedObject *)childShape);
}
