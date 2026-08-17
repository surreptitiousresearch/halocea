/* ?removeReference@hkReferencedObject@@QBAXXZ @0x838D2DE0 */
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkReferencedObjectLock.h"

/* hkReferencedObject vtable — only the destructor slot is used here. */
typedef struct hkReferencedObject_vtbl
{
    void (*dtor)(hkReferencedObject *self, int deleteFlag); /* slot 0 */
} hkReferencedObject_vtbl;

/* hkReferencedObject::removeReference — decrement the reference count and, when
   it reaches zero, invoke the virtual destructor (deleting delete, flag 1).
   Static/embedded objects (m_memSizeAndFlags == 0) are ignored. As with
   addReference the decrement is lock-guarded in LOCK_MODE_AUTO unless this
   thread already owns the lock; the drop-to-zero destruct path is taken outside
   the lock (refcount was 1, so no other thread can observe it). */
extern "C" void hkReferencedObject_removeReference(hkReferencedObject *self)
{
    hkReferencedObjectLock *lock;
    int refCount;

    if (!self->m_memSizeAndFlags)
        return;

    refCount = self->m_referenceCount;
    if (refCount == 1)
    {
        self->m_referenceCount = 0;
        ((hkReferencedObject_vtbl *)self->__vftable)->dtor(self, 1);
        return;
    }

    lock = hkReferencedObjectLock_getInstance();
    if (lock->m_lockMode != LOCK_MODE_AUTO || hkRefLock_threadOwns())
    {
        self->m_referenceCount = refCount - 1;
        return;
    }

    hkCriticalSection_enter(&lock->m_criticalSection);
    lock->m_lockCount = 1;
    hkRefLock_setThreadOwns();
    --self->m_referenceCount;
    if (--lock->m_lockCount <= 0)
    {
        hkRefLock_clearThreadOwns();
        hkCriticalSection_leave(&lock->m_criticalSection);
    }
}
