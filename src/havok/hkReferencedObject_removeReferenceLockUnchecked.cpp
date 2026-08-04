#include "../headers/havok/hkReferencedObject.h"

/* hkReferencedObject vtable — only the destructor slot is used here. */
typedef struct hkReferencedObject_vtbl
{
    void (*dtor)(hkReferencedObject *self, int deleteFlag); /* slot 0 */
} hkReferencedObject_vtbl;

/* hkReferencedObject::removeReferenceLockUnchecked — like removeReference but
   without the reference-lock critical section (the caller already holds it, or
   guarantees single-threaded access). Decrement the count and, if it was 1
   (dropping to zero), invoke the virtual destructor (deleting delete, flag 1).
   Static/embedded objects (m_memSizeAndFlags == 0) are ignored. */
void hkReferencedObject_removeReferenceLockUnchecked(hkReferencedObject *self)
{
    short refCount;

    if (!self->m_memSizeAndFlags)
        return;

    refCount = self->m_referenceCount;
    self->m_referenceCount = refCount - 1;
    if (refCount == 1)
        ((hkReferencedObject_vtbl *)self->__vftable)->dtor(self, 1);
}
