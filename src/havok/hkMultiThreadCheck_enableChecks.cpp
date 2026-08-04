#include "../headers/havok/hkReferencedObjectLock.h"

/* hkMultiThreadCheck::enableChecks — re-arm the single-threaded access checker.
   Only transitions from the "disabled" sentinel (-47) back to the "unmarked/idle"
   sentinel (-15); if checks were already enabled or a thread owns the object this
   is a no-op. Clears the recursion mark counter. */
void hkMultiThreadCheck_enableChecks(hkMultiThreadCheck *self)
{
    if (self->m_threadId == (unsigned int)-47)
    {
        self->m_threadId = (unsigned int)-15;
        self->m_markCount = 0;
    }
}
