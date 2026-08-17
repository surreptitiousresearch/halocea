/* ?addReference@hkReferencedObject@@QBAXXZ @0x838D2D30 */
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkReferencedObjectLock.h"

/* hkReferencedObject::addReference — increment the reference count. Objects with
   a zero m_memSizeAndFlags are static/embedded and are never refcounted (early
   out). In LOCK_MODE_AUTO the increment is serialised through the global lock
   unless this thread already holds it (reentrancy tag), which keeps the
   fast path lock-free for nested add/removeReference. */
extern "C" void hkReferencedObject_addReference(hkReferencedObject *self)
{
    hkReferencedObjectLock *lock;

    if (!self->m_memSizeAndFlags)
        return;

    lock = hkReferencedObjectLock_getInstance();
    if (lock->m_lockMode != LOCK_MODE_AUTO || hkRefLock_threadOwns())
    {
        ++self->m_referenceCount;
        return;
    }

    hkCriticalSection_enter(&lock->m_criticalSection);
    lock->m_lockCount = 1;
    hkRefLock_setThreadOwns();
    ++self->m_referenceCount;
    if (--lock->m_lockCount <= 0)
    {
        hkRefLock_clearThreadOwns();
        hkCriticalSection_leave(&lock->m_criticalSection);
    }
}
