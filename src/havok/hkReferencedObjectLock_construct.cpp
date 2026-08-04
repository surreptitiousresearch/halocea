#include "../headers/havok/hkReferencedObjectLock.h"

/* boundary — this object's vtable symbol (hkReferencedObjectLock::`vftable'). */
extern void *const hkReferencedObjectLock_vftable;

/* hkReferencedObjectLock::hkReferencedObjectLock — construct the process-wide
   refcount lock: refcount 1, single-thread ownership tag (-47 == "unowned"),
   a shared critical section (spin count 4000), AUTO lock mode, checks enabled,
   lock depth 0. */
void hkReferencedObjectLock_construct(hkReferencedObjectLock *self)
{
    self->base.__vftable = (hkBaseObject_vtbl *)&hkReferencedObjectLock_vftable;
    self->base.m_referenceCount = 1;
    self->m_multiThreadCheck.m_threadId = (unsigned int)-47;
    self->m_multiThreadCheck.m_markCount = 0;
    hkCriticalSection_construct(&self->m_criticalSection, 4000, 1);
    self->m_lockMode = LOCK_MODE_AUTO;
    hkMultiThreadCheck_enableChecks(&self->m_multiThreadCheck);
    self->m_lockCount = 0;
}
