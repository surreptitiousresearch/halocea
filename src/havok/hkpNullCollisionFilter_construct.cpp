#include "../headers/havok/hkpNullCollisionFilter.h"

/* hkpNullCollisionFilter::hkpNullCollisionFilter — run the collision-filter base
   constructor, then overwrite all five interface vtable pointers with the null
   filter's own tables (every isCollisionEnabled override returns true) and tag the
   filter type as HK_FILTER_NULL. */
void hkpNullCollisionFilter_construct(hkpNullCollisionFilter *self)
{
    hkpCollisionFilter_construct(self);
    self->base.__vftable                = (hkBaseObject_vtbl *)&hkpNullCollisionFilter_vftable_hkReferencedObject; /* base is the hkReferencedObject sub-object directly */
    self->m_collidableCollidableFilter_vftable = &hkpNullCollisionFilter_vftable_hkpCollidableCollidableFilter;
    self->m_rayShapeCollectionFilter_vftable   = &hkpNullCollisionFilter_vftable_hkpRayShapeCollectionFilter;
    self->m_shapeCollectionFilter_vftable      = &hkpNullCollisionFilter_vftable_hkpShapeCollectionFilter;
    self->m_rayCollidableFilter_vftable        = &hkpNullCollisionFilter_vftable_hkpRayCollidableFilter;
    self->m_type.m_storage = HK_FILTER_NULL;
}
