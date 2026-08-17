/* ??1?$hkPointerMapBase@KKU?$hkPointerMapOperations@KK@@@@QAA@XZ @0x838E49A8 */
#include "../headers/havok/hkPointerMapBase.h"

/* hkPointerMapBase::~hkPointerMapBase — free the slot buffer unless it is
   caller-owned. m_numElems >= 0 means the map allocated its own storage (the
   DONT_DEALLOCATE sign bit is clear), so return the chunk to the per-thread
   allocator. The buffer is (m_hashMod + 1) Pairs of 8 bytes each. */
void hkPointerMapBase_destruct(hkPointerMapBase *self)
{
    if (self->m_numElems >= 0)
        /* DEVIATION: the allocator is fetched at the call site as the opaque TLS
           deref *(hkThreadMemory**)(*(int*)r13 + 704); modeled via the accessor. */
        hkThreadMemory_deallocateChunk(
            hkThreadMemory_getCurrent(),
            self->m_elem,
            8 * (self->m_hashMod + 1),
            HK_MEMORY_CLASS_ARRAY);
}
