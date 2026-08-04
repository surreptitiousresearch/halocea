#include "../headers/havok/hkPointerMapBase.h"
#include "../headers/havok/hkThreadMemory.h"

/* hkPointerMapBase::hkPointerMapBase() — default constructor. Allocate the
   initial 16-slot table (128 bytes, 8 bytes per Pair) from the current thread's
   allocator, mark every slot empty (key = -1), and set m_hashMod to the
   power-of-two slot mask (15). m_numElems starts at 0 with its DONT_DEALLOCATE
   sign bit cleared, so this heap buffer is owned and freed by the destructor. */
void hkPointerMapBase_construct_default(hkPointerMapBase *self)
{
    int i;

    self->m_elem = (hkPointerMapPair *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 128, HK_MEMORY_CLASS_ARRAY);
    self->m_numElems = 0;
    self->m_hashMod = 15;
    for (i = 0; i < 16; ++i)
        self->m_elem[i].key = (unsigned int)-1;
    self->m_numElems &= (int)0x80000000; /* keep only DONT_DEALLOCATE bit (=> 0) */
}
