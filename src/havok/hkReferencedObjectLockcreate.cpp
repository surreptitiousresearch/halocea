/* hkReferencedObjectLockcreate @0x838D2BB8 */
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"

typedef struct hkReferencedObjectLock hkReferencedObjectLock;
extern void hkReferencedObjectLock_construct(hkReferencedObjectLock *self); /* hkReferencedObjectLock::hkReferencedObjectLock */

/* Factory: allocate a 64-byte hkReferencedObjectLock, stamp its allocation
   header, run its constructor, and return it. DEVIATION: the decompiler renders
   this void, but the binary returns the constructed object in r3 (0x838D2BD4
   lwzx r3 -> allocateChunk -> ctor -> blr with r3 live), matching the DB's
   pointer-returning prototype. */
hkReferencedObjectLock *hkReferencedObjectLockcreate(void)
{
    hkReferencedObjectLock *self = (hkReferencedObjectLock *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 64, HK_MEMORY_CLASS_BASE);
    ((hkReferencedObject *)self)->m_memSizeAndFlags = 64;
    hkReferencedObjectLock_construct(self);
    return self;
}
