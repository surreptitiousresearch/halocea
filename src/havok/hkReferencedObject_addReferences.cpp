#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkReferencedObjectLock.h"

/* hkReferencedObject::addReferences — batch increment of a strided array of object
   pointers. `objects` points at the first hkReferencedObject* ; successive pointers
   are `pointerStriding` bytes apart (so callers can walk an array of structs that
   merely embed the pointer). Objects with a zero m_memSizeAndFlags are
   static/embedded and skipped. As with the single addReference, when the global
   lock is engaged (mode != NONE) and this thread does not already own it, the
   whole batch is serialised through the critical section.

   DEVIATION: disasm 0x838D2EE4 tests m_lockMode against 0 (== LOCK_MODE_NONE) for
   the lock-free path — NOT `!= LOCK_MODE_AUTO` as the single-object addReference
   does; reproduced faithfully. The reentrancy tag and lock counter are wrapped in
   the hkRefLock_* accessors (see hkReferencedObjectLock.h). */
extern "C" void hkReferencedObject_addReferences(const hkReferencedObject *const *objects,
                                      int numObjects, int pointerStriding)
{
    hkReferencedObjectLock *lock = hkReferencedObjectLock_getInstance();
    int i;

    if (lock->m_lockMode == LOCK_MODE_NONE || hkRefLock_threadOwns())
    {
        for (i = 0; i < numObjects; ++i)
        {
            hkReferencedObject *obj = (hkReferencedObject *)*objects;
            if (obj->m_memSizeAndFlags)
                ++obj->m_referenceCount;
            objects = (const hkReferencedObject *const *)((const char *)objects + pointerStriding);
        }
        return;
    }

    hkCriticalSection_enter(&lock->m_criticalSection);
    lock->m_lockCount = 1;
    hkRefLock_setThreadOwns();
    for (i = 0; i < numObjects; ++i)
    {
        hkReferencedObject *obj = (hkReferencedObject *)*objects;
        if (obj->m_memSizeAndFlags)
            ++obj->m_referenceCount;
        objects = (const hkReferencedObject *const *)((const char *)objects + pointerStriding);
    }
    if (--lock->m_lockCount <= 0)
    {
        hkRefLock_clearThreadOwns();
        hkCriticalSection_leave(&lock->m_criticalSection);
    }
}
