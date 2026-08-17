/* ?addChild@hkdBreakableShape@@QAAXPBV1@ABVhkTransform@@@Z @0x83849990 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkReferencedObject.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

#define HK_REAL_MAX 3.402823466e38f /* FLT_MAX sentinel for "not yet computed" */

/* hkdBreakableShape::addChild — append a child instance (shape + placement) to
   m_children, growing the array when full, then reference the child and set its
   parent back-pointer. The runtime info is initialised to its "unevaluated"
   defaults (distance = FLT_MAX, oldChildIdx = 0xFFFF, forceFixed = 0). */
void hkdBreakableShape_addChild(hkdBreakableShape *self, const hkdBreakableShape *child, const hkTransform *transform)
{
    hkdShapeInstanceInfo *info;
    if (self->m_children.m_size == (self->m_children.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_children.m_data, 80); /* sizeof(hkdShapeInstanceInfo) */
    info = &self->m_children.m_data[self->m_children.m_size++];
    info->m_transform = *transform;
    info->m_shape = child;
    info->m_damage = 0;
    info->m_runtimeInfo.m_distanceToDestructionPoint = HK_REAL_MAX;
    info->m_runtimeInfo.m_oldChildIdx = 0xFFFF;
    info->m_runtimeInfo.m_forceFixed = 0;
    hkReferencedObject_addReference((hkReferencedObject *)child);
    ((hkdBreakableShape *)child)->m_parent = self;
}
