/* FUNCTION_INDEX entry: hkpCollidable_ctor @0x829197F8 (??0hkpCollidable@@QAA@PBVhkpShape@@PBVhkTransform@@H@Z) */
#include "../headers/havok/hkpCollidable.h"
#include "../headers/havok/hkTransform.h"

/* boundary — the bounding-volume sub-object constructor (level-2 callee). */
extern void hkpCollidable_BoundingVolumeData_construct(BoundingVolumeData *self); /* hkpCollidable::BoundingVolumeData::BoundingVolumeData */

/* hkpCollidable::hkpCollidable — initialise a collidable over a shape at a
   transform, tagging its typed broadphase handle with the given broadphase
   type. DEVIATION: the 4th argument is the broadphase-handle type byte
   (the frontier extern loosely spelled it "void *motion"); the transform
   pointer is what is stored in the hkpCdBody m_motion slot. DEVIATION: the
   original's debug assert that the this<->handle owner offset fits in a
   signed byte (via hkErrStream/hkError) is elided; the resulting owner offset
   (-20, i.e. this - &m_broadPhaseHandle) is stored directly as the code does. */
void hkpCollidable_ctor(hkpCollidable *self, const hkpShape *shape,
                        const hkTransform *t, char type)
{
    self->base.m_shape = shape;
    self->base.m_motion = t;
    self->base.m_parent = 0;
    self->base.m_shapeKey = (unsigned int)-1;
    self->m_ownerOffset = 0;

    self->m_broadPhaseHandle.m_type = type;
    self->m_broadPhaseHandle.base.m_id = 0;
    self->m_broadPhaseHandle.m_collisionFilterInfo = 0;
    self->m_broadPhaseHandle.m_objectQualityType = -1;
    self->m_broadPhaseHandle.m_ownerOffset = 127;

    hkpCollidable_BoundingVolumeData_construct(&self->m_boundingVolumeData);
    self->m_allowedPenetrationDepth = -1.0f;

    self->m_broadPhaseHandle.m_ownerOffset =
        (char)((char *)self - (char *)&self->m_broadPhaseHandle);
    self->m_forceCollideOntoPpu = 0;
    self->m_shapeSizeOnSpu = 0;
}
