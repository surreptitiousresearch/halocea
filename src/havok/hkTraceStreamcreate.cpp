/* hkTraceStreamcreate @0x838E59F8 */
#include "../headers/havok/hkTraceStream.h"
#include "../headers/havok/hkThreadMemory.h"

extern hkBaseObject_vtbl hkTraceStream_vftable; /* hkTraceStream::`vftable' */

/* Factory: allocate and inline-initialise a 28-byte hkTraceStream — dummy
   vtable, one reference, null stream/counter, and an empty title array flagged
   dont-deallocate (0x80000000). */
hkReferencedObject *hkTraceStreamcreate(void)
{
    hkTraceStream *self = (hkTraceStream *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 28, HK_MEMORY_CLASS_BASE_CLASS);
    self->m_memSizeAndFlags = 28;
    self->__vftable = &hkTraceStream_vftable;
    self->m_referenceCount = 1;
    self->m_stream = 0;
    self->m_counter = 0;
    self->m_titles.m_data = 0;
    self->m_titles.m_size = 0;
    self->m_titles.m_capacityAndFlags = 0x80000000;
    return (hkReferencedObject *)self;
}
