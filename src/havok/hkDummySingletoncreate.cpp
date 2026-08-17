/* hkDummySingletoncreate @0x838DE3C8 */
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"

extern hkBaseObject_vtbl hkDummySingleton_vftable; /* hkDummySingleton::`vftable' */

/* Factory: allocate and inline-initialise an 8-byte hkDummySingleton (bare
   hkReferencedObject with the dummy vtable, one reference). */
hkReferencedObject *hkDummySingletoncreate(void)
{
    hkReferencedObject *self = (hkReferencedObject *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 8, HK_MEMORY_CLASS_BASE_CLASS);
    self->m_memSizeAndFlags = 8;
    self->__vftable = &hkDummySingleton_vftable;
    self->m_referenceCount = 1;
    return self;
}
