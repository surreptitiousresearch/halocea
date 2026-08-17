/* ?addPhantom@hkpPhysicsSystem@@QAAXPAVhkpPhantom@@@Z @0x83903570 */
#include "../headers/havok/hkpPhysicsSystem.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkArray.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

/* hkpPhysicsSystem::addPhantom — reference and append to m_phantoms (offset 44). */
void hkpPhysicsSystem_addPhantom(hkpPhysicsSystem *self, hkpPhantom *phantom)
{
    if (!phantom)
        return;
    hkReferencedObject_addReference((hkReferencedObject *)phantom);
    if (self->m_phantoms.m_size == (self->m_phantoms.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_phantoms, 4);
    self->m_phantoms.m_data[self->m_phantoms.m_size++] = phantom;
}
