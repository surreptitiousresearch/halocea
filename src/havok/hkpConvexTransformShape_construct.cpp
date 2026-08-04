#include "../headers/havok/hkpConvexTransformShape.h"
#include "../headers/havok/hkpConvexShape.h"

/* hkpConvexTransformShape::`vftable' (boundary data). */
extern void *hkpConvexTransformShape_vftable[]; /* hkpConvexTransformShape::`vftable' */

/* Base sub-object constructor (now re-sourced): tags the shape as
   HK_SHAPE_CONVEX_TRANSFORM, records the child's radius, and adopts the child
   shape under referencePolicy. Its true ABI takes NO transform (declared in
   hkpConvexTransformShapeBase.h, included via hkpConvexTransformShape.h). */

#define HK_SHAPE_CONVEX_TRANSFORM 12

/* hkpConvexTransformShape::hkpConvexTransformShape(childShape, transform, ref) —
   run the base constructor (which does the shape/container/radius setup), install
   the derived vtable, and store the transform.

   DEVIATION: the transform store is a VMX128 lvx128/stvx128 block in the DB;
   reproduced here as a structure copy. */
void hkpConvexTransformShape_construct(hkpConvexTransformShape *self, const hkpShape *childShape,
                                       const hkTransform *transform, int referencePolicy)
{
    hkpConvexTransformShapeBase_construct(&self->base, HK_SHAPE_CONVEX_TRANSFORM,
                                          ((const hkpConvexShape *)childShape)->m_radius,
                                          childShape, referencePolicy);
    *(void **)&self->base = (void *)hkpConvexTransformShape_vftable;
    self->m_transform = *transform;
}
