/* ?removeReferences@hkReferencedObject@@SAXPBQBV1@HH@Z @0x838D2FB0 */
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkReferencedObjectLock.h"

/* The virtual table's slot 0 is the (deleting) destructor: called with arg 1 to
   run destruction + free once the reference count reaches zero. */
struct hkReferencedObject_vtbl { void (*destructor)(void *self, int deleteFlag); };

/* Decrement one reference on each object in a strided array; destroy any object
   whose count reaches zero. Shared by the locked and unlocked paths. */
static void removeRefsLoop(const hkReferencedObject *const *objects, int numObjects, int pointerStriding)
{
    while (numObjects > 0)
    {
        hkReferencedObject *obj = (hkReferencedObject *)*objects;
        if (obj->m_memSizeAndFlags)
        {
            short old = obj->m_referenceCount;
            obj->m_referenceCount = old - 1;
            if (old == 1)
            {
                struct hkReferencedObject_vtbl **vtbl =
                    (struct hkReferencedObject_vtbl **)obj;
                (*vtbl)->destructor(obj, 1);
            }
        }
        --numObjects;
        objects = (const hkReferencedObject *const *)((const char *)objects + pointerStriding);
    }
}

/* hkReferencedObject::removeReferences — batch removeReference over an array of
   object pointers spaced pointerStriding bytes apart. When the global lock is
   inactive (or this thread already owns the teardown guard) the counts are
   adjusted directly; otherwise the critical section is taken and the per-thread
   reentrancy tag set for the duration. */
void hkReferencedObject_removeReferences(void *objects, int numObjects, int pointerStriding)
{
    const hkReferencedObject *const *objs = (const hkReferencedObject *const *)objects;
    hkReferencedObjectLock *lock = hkReferencedObjectLock_getInstance();

    if (lock->m_lockMode == LOCK_MODE_NONE || hkRefLock_threadOwns())
    {
        removeRefsLoop(objs, numObjects, pointerStriding);
    }
    else
    {
        int remaining;
        hkCriticalSection_enter(&lock->m_criticalSection);
        lock->m_lockCount = 1;
        hkRefLock_setThreadOwns();
        removeRefsLoop(objs, numObjects, pointerStriding);
        remaining = lock->m_lockCount - 1;
        lock->m_lockCount = remaining;
        if (remaining <= 0)
        {
            hkRefLock_clearThreadOwns();
            hkCriticalSection_leave(&lock->m_criticalSection);
        }
    }
}
