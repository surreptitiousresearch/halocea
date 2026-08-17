/* hkSocketCreate @0x838DF050 */
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"

typedef struct hkBsdSocket hkBsdSocket;
extern void hkBsdSocket_construct(hkBsdSocket *self, unsigned int socket); /* hkBsdSocket::hkBsdSocket; -1 = invalid handle */

/* Factory: allocate a 36-byte hkBsdSocket, stamp its allocation header (hkSocket
   carries the standard hkReferencedObject header at offset 0), construct it with
   an invalid socket handle, and return it. DEVIATION: decompiler renders void,
   but the binary returns the socket in r3 (0x838DF06C lwzx r3 -> allocateChunk
   -> ctor -> blr; sole caller hkVisualDebugger::serve stores r3), matching the
   DB prototype `hkSocket *hkSocketCreate()`. */
hkBsdSocket *hkSocketCreate(void)
{
    hkBsdSocket *self = (hkBsdSocket *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 36, HK_MEMORY_CLASS_BASE_CLASS);
    ((hkReferencedObject *)self)->m_memSizeAndFlags = 36;
    hkBsdSocket_construct(self, 0xFFFFFFFF);
    return self;
}
