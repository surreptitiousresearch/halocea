#include "../headers/havok/hkReferencedObjectLock.h"

/* hkSingleton<hkReferencedObjectLock>::s_instance — the process-wide refcount
   lock instance pointer (a data-segment global). */
extern hkReferencedObjectLock *hkSingleton_hkReferencedObjectLock_s_instance;

/* hkSingleton<hkReferencedObjectLock>::getInstance — return the singleton. */
hkReferencedObjectLock *hkReferencedObjectLock_getInstance(void)
{
    return hkSingleton_hkReferencedObjectLock_s_instance;
}
