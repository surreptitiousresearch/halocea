#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"

typedef struct hkReferencedObjectLock hkReferencedObjectLock;
extern void hkReferencedObjectLock_construct(hkReferencedObjectLock *self); /* hkReferencedObjectLock::hkReferencedObjectLock */

/* Factory: allocate a 64-byte hkReferencedObjectLock, stamp its allocation
   header, run its constructor. DEVIATION: the decompiler renders this void, but
   the constructed object is returned in r3 (as with the sibling create fns). */
void hkReferencedObjectLockcreate(void)
{
    hkReferencedObjectLock *self = (hkReferencedObjectLock *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 64, HK_MEMORY_CLASS_BASE);
    ((hkReferencedObject *)self)->m_memSizeAndFlags = 64;
    hkReferencedObjectLock_construct(self);
}
