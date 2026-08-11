/* FUNCTION_INDEX entry: hkpCollidable_BoundingVolumeData_construct @0x83600340 (??0BoundingVolumeData@hkpCollidable@@QAA@XZ) */
#include "../headers/havok/hkpCollidable.h"

/* hkpCollidable::BoundingVolumeData::BoundingVolumeData — default constructor.
   Clears the cached child-AABB arrays and marks the volume invalid by setting an
   inverted min/max sentinel (m_min[0] = 1 > m_max[0] = 0). */
void hkpCollidable_BoundingVolumeData_construct(BoundingVolumeData *self)
{
    self->m_numChildShapeAabbs = 0;
    self->m_capacityChildShapeAabbs = 0;
    self->m_childShapeAabbs = nullptr;
    self->m_childShapeKeys = nullptr;
    self->m_min[0] = 1;
    self->m_max[0] = 0;
}
