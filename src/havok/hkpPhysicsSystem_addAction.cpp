#include "../headers/havok/hkpPhysicsSystem.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkArray.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

/* hkpPhysicsSystem::addAction — reference and append to m_actions (offset 32).
   DEVIATION: the decompiler mistyped `this` as D3DDevice and reached the array
   as m_Pending.m_Mask[4]; it is m_actions. */
void hkpPhysicsSystem_addAction(hkpPhysicsSystem *self, hkpAction *action)
{
    if (!action)
        return;
    hkReferencedObject_addReference((hkReferencedObject *)action);
    if (self->m_actions.m_size == (self->m_actions.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_actions, 4);
    self->m_actions.m_data[self->m_actions.m_size++] = action;
}
