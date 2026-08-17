/* ?addRigidBody@hkpPhysicsSystem@@QAAXPAVhkpRigidBody@@@Z @0x839034E8 */
#include "../headers/havok/hkpPhysicsSystem.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkArray.h"

extern void hkArrayUtil__reserveMore(void *array, int sizeElem); /* hkArrayUtil::_reserveMore */

/* hkpPhysicsSystem::addRigidBody — take a reference on the body and push it onto
   m_rigidBodies (growing the array first if it is full). Null bodies are
   ignored. DEVIATION: the decompiler mistyped `this` as D3DDevice and reached
   the array as m_Pending.m_Mask[1]; it is m_rigidBodies at offset 8. */
void hkpPhysicsSystem_addRigidBody(hkpPhysicsSystem *self, hkpRigidBody *body)
{
    if (!body)
        return;
    hkReferencedObject_addReference((hkReferencedObject *)body);
    if (self->m_rigidBodies.m_size == (self->m_rigidBodies.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&self->m_rigidBodies, 4);
    self->m_rigidBodies.m_data[self->m_rigidBodies.m_size++] = body;
}
