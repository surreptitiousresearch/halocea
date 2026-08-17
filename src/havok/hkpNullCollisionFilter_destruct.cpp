/* ??1hkpNullCollisionFilter@@UAA@XZ @0x83618858 */
#include "../headers/havok/hkpNullCollisionFilter.h"

/* hkpNullCollisionFilter::~hkpNullCollisionFilter — devirtualize the object back
   to its interface base vtables (standard C++ base-class destructor teardown
   order). No owned resources to release. */
void hkpNullCollisionFilter_destruct(hkpNullCollisionFilter *self)
{
    self->m_rayCollidableFilter_vftable        = &hkpRayCollidableFilter_vftable;
    self->m_rayShapeCollectionFilter_vftable   = &hkpRayShapeCollectionFilter_vftable;
    self->m_shapeCollectionFilter_vftable      = &hkpShapeCollectionFilter_vftable;
    self->m_collidableCollidableFilter_vftable = &hkpCollidableCollidableFilter_vftable;
    self->base.__vftable                  = (hkBaseObject_vtbl *)&hkBaseObject_vftable; /* base is the hkReferencedObject sub-object directly */
}
