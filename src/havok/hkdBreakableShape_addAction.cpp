/* ?addAction@hkdBreakableShape@@QAAXPAVhkdAction@@@Z @0x838498F8 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdAction.h"
#include "../headers/havok/hkReferencedObject.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

/* hkdBreakableShape::addAction — append an action to m_actions (growing the
   array when full) and take a reference on it. */
void hkdBreakableShape_addAction(hkdBreakableShape *self, hkdAction *action)
{
    if (self->m_actions.m_size == (self->m_actions.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_actions.m_data, 4); /* sizeof(hkdAction*) */
    self->m_actions.m_data[self->m_actions.m_size++] = action;
    hkReferencedObject_addReference((hkReferencedObject *)action);
}
