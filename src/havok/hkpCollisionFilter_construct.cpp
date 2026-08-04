#include "../headers/havok/hkpNullCollisionFilter.h"

/* hkpCollisionFilter's own multiply-inherited vtables (one thunk table per
   inherited interface). Boundary symbols emitted by the C++ compiler. */
extern void *hkpCollisionFilter_vftable_hkReferencedObject;
extern void *hkpCollisionFilter_vftable_hkpCollidableCollidableFilter;
extern void *hkpCollisionFilter_vftable_hkpShapeCollectionFilter;
extern void *hkpCollisionFilter_vftable_hkpRayShapeCollectionFilter;
extern void *hkpCollisionFilter_vftable_hkpRayCollidableFilter;

/* hkpCollisionFilter::hkpCollisionFilter (default ctor)
   Initialises the reference count and installs the five interface vtables. The
   compiler first stores each interface's own default vtable (the base-subobject
   construction step) and then overwrites them with hkpCollisionFilter's derived
   thunk vtables; both stores are reproduced in source order for fidelity. The
   filter type is left cleared (callers such as the null filter tag it afterwards). */
void hkpCollisionFilter_construct(hkpCollisionFilter *self)
{
    self->base.m_referenceCount = 1;

    /* base-subobject default vtables */
    self->m_collidableCollidableFilter_vftable = &hkpCollidableCollidableFilter_vftable;
    self->m_shapeCollectionFilter_vftable      = &hkpShapeCollectionFilter_vftable;
    self->m_rayShapeCollectionFilter_vftable   = &hkpRayShapeCollectionFilter_vftable;
    self->m_rayCollidableFilter_vftable        = &hkpRayCollidableFilter_vftable;

    /* derived hkpCollisionFilter thunk vtables (final) */
    self->base.__vftable                       = (hkBaseObject_vtbl *)&hkpCollisionFilter_vftable_hkReferencedObject;
    self->m_collidableCollidableFilter_vftable = &hkpCollisionFilter_vftable_hkpCollidableCollidableFilter;
    self->m_shapeCollectionFilter_vftable      = &hkpCollisionFilter_vftable_hkpShapeCollectionFilter;
    self->m_rayShapeCollectionFilter_vftable   = &hkpCollisionFilter_vftable_hkpRayShapeCollectionFilter;
    self->m_rayCollidableFilter_vftable        = &hkpCollisionFilter_vftable_hkpRayCollidableFilter;

    self->m_type.m_storage = 0;
}
